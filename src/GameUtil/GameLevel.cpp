#include "GameUtil/GameLevel.h"
#include "GameUtil/ResourceManager.h"

namespace GameUtil{
    GameLevel::GameLevel(){}
    void GameLevel::Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight, ResourceManager *source)
    {
        this->Bricks.clear();
        GLuint tileCode;
        GameLevel level;
        std::string line;
        std::ifstream fstream(file);
        std::vector<std::vector<GLuint>> tileData;
        if(fstream){
            while(std::getline(fstream,line)){
                std::istringstream sstream(line);
                std::vector<GLuint> row;
                while(sstream >> tileCode)
                    row.push_back(tileCode);
                tileData.push_back(row);
            }

            if(tileData.size()>0)
                this->initialize(tileData, levelWidth, levelHeight,source);
        }
    }

    void
    GameLevel::initialize(
        std::vector<std::vector<GLuint>> tileData,
        GLuint levelWidth, GLuint levelHeight, ResourceManager *source)
    {
        GLuint height = tileData.size();
        GLuint width = tileData[0].size();

        GLfloat unitWidth = levelWidth / static_cast<GLfloat>(width);
        GLfloat unitHeight = levelHeight / static_cast<GLfloat>(height);
        for (GLuint y = 0; y < height;y++){
            for (GLuint x = 0; x < width;x++){
                if(tileData[y][x]==1){
                    glm::vec2 pos(unitWidth * x, unitHeight * y);
                    glm::vec2 size(unitWidth, unitHeight);

                    GameObject gameObj(pos, size,
                                       source->GetTexture("block_solid"),
                                       glm::vec3(0.8f, 0.8f, 0.7f));
                    gameObj.IsSolid = true;
                    this->Bricks.push_back(gameObj);
                }else if(tileData[y][x]>1){
                    glm::vec3 color = glm::vec3(1.0f);
                    if(tileData[y][x]==2)
                        color = glm::vec3(0.2f, 0.6f, 1.0f);
                    else if(tileData[y][x]==3)
                        color = glm::vec3(0.0f, 0.7f, 0.0f);
                    else if(tileData[y][x]==4)
                        color = glm::vec3(0.8f, 0.8f, 0.4f);
                    else if(tileData[y][x]==5){
                        color = glm::vec3(1.0f, 0.5f, 0.0f);
                    }

                    glm::vec2 pos(unitWidth * x, unitHeight * y);
                    glm::vec2 size(unitWidth, unitHeight);
                    this->Bricks.push_back(GameObject(pos, size, source->GetTexture("block"), color));
                }
            }
        }
    }

    void GameLevel::Draw(SpriteRenderer& renderer){
        for(GameObject& tile:this->Bricks)
            if(!tile.Destroyed)
                tile.Draw(renderer);
    }

    GLboolean GameLevel::IsCompleted(){
        for(GameObject &tile:this->Bricks)
            if(!tile.IsSolid && !tile.Destroyed)
                return false;
        return true;
    }

}