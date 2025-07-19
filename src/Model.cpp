#include "Model.h"

// Constructor to load a model from a model file
Model::Model(const char* path)
{
	// Simply get absolute path to ensure no file not found issues
	std::filesystem::path absolute = std::filesystem::absolute(path);
	loadModel(absolute.string()); // Load the model
	initialiseBounds(); // Initialise OOBB
}

// Constructor to copy data from one model to another
// E.g. Model m2 = m1;
Model::Model(const Model& model)
{
	this->meshes = model.meshes;
	initialiseBounds();
}

// Delete all data when done
Model::~Model()
{
	for (Mesh& mesh : meshes)
	{
		mesh.remove();
	}
}

// Simply loop through all models and draw individually
// Set the model matrix to the combined rotation, translation and scale matrices
void Model::draw(Shader& shader)
{
	if (visible)
	{
		shader.activate();
		shader.setMat4("model", getModel());

		for (Mesh& mesh : meshes)
		{
			mesh.draw(shader);
		}
	}
}

// Function to use assimp to load a model
void Model::loadModel(const std::string& path)
{
	Assimp::Importer importer;
	// Triangulate all models as only work with triangles in following code
	// Generate smooth normals to ensure better lighting
	// Calculate tangents so that all calculations are done in TBN space
	const aiScene* scene = importer.ReadFile(path, 
		aiProcess_Triangulate | aiProcess_GenSmoothNormals | 
		aiProcess_CalcTangentSpace
	);

	if (!scene || scene->mFlags && AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('\\'));
	// Start processing from the root node of the scene graph
	processNode(scene->mRootNode, scene);
}

// Assimp data structure defines the models as a scene graph
// Here, we process each node of the graph
void Model::processNode(aiNode *node, const aiScene *scene)
{
	// Simply process all meshes at current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* aimesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(aimesh, scene));
	}

	// Recursively process child nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

// Simply load all data from the assimp mesh into my own mesh class
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	// No need for direct checking for normals etc existance due to models used
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex(
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z
		);
		
		vertex.normal = glm::vec3(
			mesh->mNormals[i].x,
			mesh->mNormals[i].y,
			mesh->mNormals[i].z
		);
		
		vertex.texCoords = glm::vec2(
			mesh->mTextureCoords[0][i].x,
			mesh->mTextureCoords[0][i].y
		);

		vertex.tangent = glm::vec3(
			mesh->mTangents[i].x,
			mesh->mTangents[i].y,
			mesh->mTangents[i].z
		);
		
		vertices.push_back(vertex);
	}

	// Get indices for use with EBO
	// Avoids storing unneccessary vertices
	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// Get all textures attatches to the current mesh
	if (mesh->mMaterialIndex >= 0)
	{
		Material meshMaterial(scene->mMaterials[mesh->mMaterialIndex], directory);
		textures = meshMaterial.getTextures();
	}
	return Mesh(vertices, indices, textures);
}

// Initialise bounding box to model extents
void Model::initialiseBounds()
{
	glm::vec3 minBounds = glm::vec3(std::numeric_limits<float>::max());
	glm::vec3 maxBounds = glm::vec3(std::numeric_limits<float>::lowest());
	for (const Mesh& mesh : meshes)
	{
		for (const Vertex& vertex : mesh.vertices)
		{
			minBounds.x = std::min(minBounds.x, vertex.position.x);
			minBounds.y = std::min(minBounds.y, vertex.position.y);
			minBounds.z = std::min(minBounds.z, vertex.position.z);

			maxBounds.x = std::max(maxBounds.x, vertex.position.x);
			maxBounds.y = std::max(maxBounds.y, vertex.position.y);
			maxBounds.z = std::max(maxBounds.z, vertex.position.z);
		}
	}
	boundingBox.setBounds(minBounds, maxBounds);
}
