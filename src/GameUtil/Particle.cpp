#include "GameUtil/Particle.h"


namespace GameUtil{
    ParticleGenerator::ParticleGenerator(Shader shader,Texture2d texture,GLuint amount)
        :shader(shader),texture(texture),amount(amount)
        {
            this->init();
        }
    void ParticleGenerator::Update(GLfloat dt,GameObject& object,GLuint newParticles,glm::vec2 offset)
    {
            for (GLuint i = 0; i < newParticles;i++)
            {
                int unusedParticle = this->firstUnusedParticle();
                this->respawnParticle(this->particles[unusedParticle], object, offset);
            }

            for (GLuint i = 0; i < this->amount;i++){
                Particle &p = this->particles[i];
                p.Life -= dt;
                if(p.Life>0.0f){
                    p.Position -= p.Velocity * dt;
                    p.Color.a -= dt * 2.5;
                }
            }
    }

    void ParticleGenerator::Draw()
    {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            this->shader.Use();

            for(Particle particle:this->particles){
                if(particle.Life>0.0f)
                {
                    this->shader.setVec2("offset", particle.Position);
                    this->shader.setVec4("color", particle.Color);
                    this->texture.Bind();
                    glBindVertexArray(this->VAO);
                    glDrawArrays(GL_TRIANGLES, 0, 6);
                    glBindVertexArray(0);
                }
            }
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void ParticleGenerator::init(){
            GLuint VBO;
            GLfloat particleQuad[] = {
                0.0f, 1.0f, 0.0f, 1.0f,
                1.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f,

                0.0f, 1.0f, 0.0f, 1.0f,
                1.0f, 1.0f, 1.0f, 1.0f,
                1.0f, 0.0f, 1.0f, 0.0f};

            glGenVertexArrays(1, &this->VAO);
            glGenBuffers(1, &VBO);
            glBindVertexArray(this->VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(particleQuad), particleQuad, GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid *)0);
            glBindVertexArray(0);

            for (GLuint i = 0; i < this->amount;i++)
                this->particles.push_back(Particle());
    }

    GLuint lastUsedParticle = 0;
    GLuint ParticleGenerator::firstUnusedParticle()
    {
            for (GLuint i = lastUsedParticle; i < this->amount;i++)
            {
                if(this->particles[i].Life<=0.0f){
                    lastUsedParticle = i;
                    return i;
                }
            }

            for (GLuint i = 0; i < lastUsedParticle;i++)
            {
                if(this->particles[i].Life<=0.0f){
                    lastUsedParticle = i;
                    return i;
                }
            }

            lastUsedParticle = 0;
            return 0;
    }


    void ParticleGenerator::respawnParticle(Particle& particle,GameObject& object,glm::vec2 offset)
    {
            GLfloat random = ((rand() % 100) - 50) / 10.0f;
            GLfloat rColor = 0.5f + ((rand() % 100) / 100.0f);
            particle.Position = object.Position + random + offset;
            particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
            particle.Life = 1.0f;
            particle.Velocity = object.Velocity * 0.1f;
    }
}