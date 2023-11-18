#include "PostProcessPass.h"

#include <glad/glad.h>
#include <Shader.h>

#include "Mesh.h"

PostProcessPass::PostProcessPass(Shader* coolEffectShader, unsigned int colorTexture) : shader(coolEffectShader), colorTexture(colorTexture) {
    quad = new Mesh(Mesh::GenerateQuad());
}

void PostProcessPass::Execute() {
    glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
    // clear all relevant buffers
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
    glClear(GL_COLOR_BUFFER_BIT);

    shader->use();
    shader->setInt("screenTexture", 0);
    glBindTexture(GL_TEXTURE_2D, colorTexture); // Ensure this is the texture with the rendered scene
    quad->Draw(*shader);

    glEnable(GL_DEPTH_TEST);
}