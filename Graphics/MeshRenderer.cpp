#include "MeshRenderer.h"

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

void MeshRenderer::RecalculateTransform() {
	transform = transformMatrix;
	transform = glm::rotate(transform, RotX, glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, RotY, glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, RotZ, glm::vec3(0.0f, 0.0f, 1.0f));
	transform *= scaleMatrix;
}

void MeshRenderer::Draw(glm::mat4 projection, glm::mat4 view) {
	this->mesh_shader.Use();
	mesh_shader.SetMatrix4("proj", projection);
	mesh_shader.SetMatrix4("view", view);
	mesh_shader.SetMatrix4("model", transform);
	mesh_vao.Bind();
	glDrawArrays(GL_TRIANGLES, 0, getMeshPointCount());
	mesh_vao.Unbind();

}

void MeshRenderer::Move(float posx = 0.0f, float posy = 0.0f, float posz = 0.0f) {
	transformMatrix = glm::translate(transformMatrix, glm::vec3(posx, posy, posz));
	RecalculateTransform();
}

void MeshRenderer::Scale(float scalex = 1.0f, float scaley = 1.0f, float scalez = 1.0f) {
	scaleMatrix = glm::scale(scalex, scaley, scalez);
	RecalculateTransform();
}

void MeshRenderer::Rotate(float rotx = 0.0f, float roty = 0.0f, float rotz = 0.0f) {
	RotX += rotx;
	RotY += roty;
	RotZ += rotz;
	if (RotX <= 360.0f)
		RotX -= 360.0f;
	if (RotY <= 360.0f)
		RotY -= 360.0f;
	if (RotZ <= 360.0f)
		RotZ -= 360.0f;

	RecalculateTransform();
}

void MeshRenderer::SetRotation(float rotx = 0.0f, float roty = 0.0f, float rotz = 0.0f) {
	RotX = rotx;
	RotY = roty;
	RotZ = rotz;
	if (RotX <= 360.0f)
		RotX -= 360.0f;
	if (RotY <= 360.0f)
		RotY -= 360.0f;
	if (RotZ <= 360.0f)
		RotZ -= 360.0f;

	RecalculateTransform();
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
				mesh_data.mVertices.push_back(glm::vec3(vp->x, vp->y, vp->z));
			}
			if (mesh->HasNormals()) {
				const aiVector3D* vn = &(mesh->mNormals[v_i]);
				mesh_data.mNormals.push_back(glm::vec3(vn->x, vn->y, vn->z));
			}
			if (mesh->HasTextureCoords(0)) {
				const aiVector3D* vt = &(mesh->mTextureCoords[0][v_i]);
				mesh_data.mTextureCoords.push_back(glm::vec2(vt->x, vt->y));
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
	mesh_shader.Compile(PVS, PFS);

	GLuint loc1 = glGetAttribLocation(mesh_shader.ID, "vertex_position");
	GLuint loc2 = glGetAttribLocation(mesh_shader.ID, "vertex_normal");
	GLuint loc3 = glGetAttribLocation(mesh_shader.ID, "vertex_texture");

	VBO mesh_vbo_v = VBO(mesh_data.mPointCount * sizeof(glm::vec3), &mesh_data.mVertices[0]);
	VBO mesh_vbo_f = VBO(mesh_data.mPointCount * sizeof(glm::vec3), &mesh_data.mNormals[0]);

	mesh_vao.Init();
	mesh_vao.Bind();
	mesh_vao.LinkAttrib(mesh_vbo_v, loc1, 3, GL_FLOAT, 0, 0);
	mesh_vao.LinkAttrib(mesh_vbo_f, loc2, 3, GL_FLOAT, 0, 0);
	mesh_vao.Unbind();
}

GLuint MeshRenderer::getShaderProgramID(){
	return mesh_shader.ID;
}

size_t MeshRenderer::getMeshPointCount() {
	return mesh_data.mPointCount;
}
