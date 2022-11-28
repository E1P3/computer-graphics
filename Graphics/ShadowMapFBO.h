#ifndef SHADOW_MAP_FBO_H
#define SHADOW_MAP_FBO_H


#include<glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include<glm/gtc/quaternion.hpp>
#include "shader.h"


class ShadowMapFBO {
    public:
        ShadowMapFBO()
        {
            m_fbo = 0;
            m_shadowMap = 0;
        }

        ~ShadowMapFBO()
        {
            if (m_fbo != 0) {
                glDeleteFramebuffers(1, &m_fbo);
            }

            if (m_shadowMap != 0) {
                glDeleteTextures(1, &m_shadowMap);
            }
        }

        bool Init(unsigned int Width, unsigned int Height, bool ForPCF)
        {
            m_width = Width;
            m_height = Height;

            // Create the FBO
            glGenFramebuffers(1, &m_fbo);

            // Create the depth buffer
            glGenTextures(1, &m_shadowMap);
            glBindTexture(GL_TEXTURE_2D, m_shadowMap);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Width, Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

            GLint FilterType = ForPCF ? GL_LINEAR : GL_NEAREST;

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilterType);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilterType);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);


            glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowMap, 0);

            // Disable writes to the color buffer
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);

            GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

            if (Status != GL_FRAMEBUFFER_COMPLETE) {
                printf("FB error, status: 0x%x\n", Status);
                return false;
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            return true;
        }


        void BindForWriting()
        {
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
            glViewport(0, 0, m_width, m_height);  // set the width/height of the shadow map!
        }


        void BindForReading(GLenum TextureUnit)
        {
            glActiveTexture(TextureUnit);
            glBindTexture(GL_TEXTURE_2D, m_shadowMap);
        }

        unsigned int m_width = 0;
        unsigned int m_height = 0;
        GLuint m_fbo;
        GLuint m_shadowMap;
};

#endif