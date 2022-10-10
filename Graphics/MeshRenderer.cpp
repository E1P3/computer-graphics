#include "MeshRenderer.h"

MeshRenderer::MeshRenderer() {
	
}

MeshRenderer::MeshRenderer(const char* mesh_file_name, const char* pvs_file_name, const char* pfs_file_name){
	ImportMeshData(mesh_file_name);
	ImportVertexShader(pvs_file_name);
	ImportFragmentShader(pfs_file_name);
	Render();
}

char* MeshRenderer::readShaderSource(const char* shaderFile) {
	FILE* fp;
	fopen_s(&fp, shaderFile, "rb");

	if (fp == NULL) { return NULL; }

	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);

	fseek(fp, 0L, SEEK_SET);
	char* buf = new char[size + 1];
	fread(buf, 1, size, fp);
	buf[size] = '\0';

	fclose(fp);

	return buf;
}

void MeshRenderer::ImportMeshData(const char* file_name){
	/* Use assimp to read the model file, forcing it to be read as    */
	/* triangles. The second flag (aiProcess_PreTransformVertices) is */
	/* relevant if there are multiple meshes in the model file that   */
	/* are offset from the origin. This is pre-transform them so      */
	/* they're in the right position.                                 */
	const aiScene* scene = aiImportFile(
		file_name,
		aiProcess_Triangulate
	);

	if (!scene) {
		fprintf(stderr, "ERROR: reading mesh %s\n", file_name);
	}

	printf("  %i materials\n", scene->mNumMaterials);
	printf("  %i meshes\n", scene->mNumMeshes);
	printf("  %i textures\n", scene->mNumTextures);

	for (unsigned int m_i = 0; m_i < scene->mNumMeshes; m_i++) {
		const aiMesh* mesh = scene->mMeshes[m_i];
		printf("    %i vertices in mesh\n", mesh->mNumVertices);
		mesh_data.mPointCount += mesh->mNumVertices;
		for (unsigned int v_i = 0; v_i < mesh->mNumVertices; v_i++) {
			if (mesh->HasPositions()) {
				const aiVector3D* vp = &(mesh->mVertices[v_i]);
				mesh_data.mVertices.push_back(vec3(vp->x, vp->y, vp->z));
			}
			if (mesh->HasNormals()) {
				const aiVector3D* vn = &(mesh->mNormals[v_i]);
				mesh_data.mNormals.push_back(vec3(vn->x, vn->y, vn->z));
			}
			if (mesh->HasTextureCoords(0)) {
				const aiVector3D* vt = &(mesh->mTextureCoords[0][v_i]);
				mesh_data.mTextureCoords.push_back(vec2(vt->x, vt->y));
			}
			if (mesh->HasTangentsAndBitangents()) {
				/* You can extract tangents and bitangents here              */
				/* Note that you might need to make Assimp generate this     */
				/* data for you. Take a look at the flags that aiImportFile  */
				/* can take.                                                 */
			}
		}
	}

	aiReleaseImport(scene);
}

void MeshRenderer::ImportVertexShader(const char* file_name){
	PVS = readShaderSource(file_name);
}

void MeshRenderer::ImportFragmentShader(const char* file_name){
	PFS = readShaderSource(file_name);
}

void MeshRenderer::Render(){
	mesh_shader.Init();
	mesh_shader.AddShader(PVS, GL_VERTEX_SHADER);
	mesh_shader.AddShader(PFS, GL_FRAGMENT_SHADER);
	mesh_shader.CompileAll();

	GLuint loc1 = glGetAttribLocation(mesh_shader.ID, "vertex_position");
	GLuint loc2 = glGetAttribLocation(mesh_shader.ID, "vertex_normal");
	GLuint loc3 = glGetAttribLocation(mesh_shader.ID, "vertex_texture");

	VBO mesh_vbo_v = VBO(mesh_data.mPointCount * sizeof(vec3), &mesh_data.mVertices[0]);
	VBO mesh_vbo_f = VBO(mesh_data.mPointCount * sizeof(vec3), &mesh_data.mNormals[0]);
	VAO mesh_vao;

	mesh_vao.Init();
	mesh_vao.LinkAttrib(mesh_vbo_v, loc1, 3, GL_FLOAT, 0, 0);
	mesh_vao.LinkAttrib(mesh_vbo_f, loc2, 3, GL_FLOAT, 0, 0);
}

GLuint MeshRenderer::getShaderProgramID(){
	return mesh_shader.ID;
}

size_t MeshRenderer::getMeshPointCount() {
	return mesh_data.mPointCount;
}
