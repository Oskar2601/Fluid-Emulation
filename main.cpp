#include <iostream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <raylib.h>
#include <rlgl.h>

using std::string;
using std::cout;
using std::cin;
using std::endl;



class WaterVerticeClass {
public:
   Vector2 position;
   Vector2 velocity;
   Vector2 polyArray[3];
   Color polygonColour = RED;

   bool locked = false;
   bool visible = true;

   int id = 0;

   float gravity = 1.81f;
   float pushRadius = 100.f;
   float mass = 1.f;
   float size = 10.f;

   void CreateVertice();
   void Update();
private:
   Vector2 frameVelocity;
};

std::vector<WaterVerticeClass> vertArray(0);


float GetDistance(Vector2 startPoint, Vector2 endPoint) {
   return sqrt(pow((endPoint.x - startPoint.x), 2) + pow((endPoint.y - startPoint.y), 2));
}

float Map(float val, float in1, float in2, float out1, float out2)
{
   return out1 + (val - in1) * (out2 - out1) / (in2 - in1);
}

Vector2 GetAngle(Vector2 startAngle, Vector2 endAngle) {
   float radiansAngle = atan2(endAngle.y - startAngle.y, endAngle.x - startAngle.x);
   Vector2 vectorAngle = Vector2{(float)cos(radiansAngle), (float)sin(radiansAngle)};

   return vectorAngle;
}



void WaterVerticeClass::CreateVertice() {
   id = vertArray.size() + 1;
   polyArray[0] = position;
   vertArray.resize(vertArray.size() + 1);
   vertArray[vertArray.size() - 1] = *this;
}

void WaterVerticeClass::Update() {
   float deltaTime = GetFrameTime();

   frameVelocity = Vector2{0.f, 0.f};
   if(locked) {
      return;
   }


   for(unsigned int i = 0; i < vertArray.size(); i++) {
      float distance = GetDistance(position, vertArray[i].position);
      Vector2 vertAngle = GetAngle(position, vertArray[i].position);

      if(distance == 0) {
         continue;
      }

      if(distance < vertArray[i].pushRadius) {
         frameVelocity.x += (distance - vertArray[i].pushRadius) * vertAngle.x / 30;
         frameVelocity.y += (distance - vertArray[i].pushRadius) * vertAngle.y / 30;
      }
   }


   velocity.x += 4.f / (GetDistance(position, Vector2{0, position.y}) + 1);
   velocity.x -= 4.f / (GetDistance(position, Vector2{(float)GetScreenWidth(), position.y}) + 1);
   frameVelocity.y -= 100.f / (GetDistance(position, Vector2{position.x, (float)GetScreenHeight()}) + 1);
   frameVelocity.y += gravity;

   position.x = position.x + (frameVelocity.x + (velocity.x / 100)) * deltaTime * 150;
   position.y = position.y + (frameVelocity.y + (velocity.y / 100)) * deltaTime * 150;


   velocity = Vector2{velocity.x - velocity.x / 100, velocity.y - velocity.y / 100};

   if(position.x > (float)GetScreenWidth())
      position.x = (float)GetScreenWidth();

   if(position.x < 0)
      position.x = 0;

   if(position.y > (float)GetScreenHeight()) {
      position.y = (float)GetScreenHeight();
      velocity = Vector2{velocity.x, velocity.y -5.f};
   }
   
   if(position.y < 0)
      position.y = 1;

   polyArray[0] = position;
   for(int j = 0; j < 2; j++) {
      float shortDistance = 9999999.f;

      for(unsigned int i = 0; i < vertArray.size(); i++) {
         float distance = GetDistance(position, vertArray[i].position);
         if(shortDistance > distance && polyArray[j].x != vertArray[i].position.x) {
            shortDistance = distance;
            polyArray[j + 1] = vertArray[i].position;
         }
      }
   }

}



int main(void)
{
   // Variables
   //--------------------------------------------------------------------------------------
   const int screenWidth = 1000;
   const int screenHeight = 540;

   string framesPerSecond;
   string timeSinceInitialization;

   // SetConfigFlags(FLAG_WINDOW_RESIZABLE);
   // SetConfigFlags(FLAG_MSAA_4X_HINT);
   
   InitWindow(screenWidth, screenHeight, "Fluid :D");
   rlDisableBackfaceCulling();
   SetTargetFPS(144);

   WaterVerticeClass vertice;

   // floor to push them up
   
   
   vertice.pushRadius = 400.f;
   vertice.polygonColour = BLACK;
   vertice.size = 20.f;
   vertice.CreateVertice();

   vertice.pushRadius = 100.f;
   vertice.polygonColour = RED;
   vertice.size = 10.f;
   for(unsigned int i = 0; i < 200; i++) {
      vertice.position = Vector2{((float)GetScreenWidth() / 2) + i, ((float)GetScreenHeight() / 2) + i};
      vertice.CreateVertice();
   }
   

   while (!WindowShouldClose())
   {
      timeSinceInitialization = std::to_string((int)GetTime());
      framesPerSecond = std::to_string(GetFPS());



      for(unsigned int i = 0; i < vertArray.size(); i++)
         vertArray[i].Update();

      if(IsMouseButtonPressed(0))
         vertArray[0].position = GetMousePosition();
      
      if(IsMouseButtonPressed(1))
         vertArray[1].position = GetMousePosition();

      // Rendering
      //----------------------------------------------------------------------------------
      BeginDrawing();

         ClearBackground(RAYWHITE);

         for(unsigned int i = 0; i < vertArray.size(); i++) {
            if(vertArray[i].visible) {
               DrawCircle( vertArray[i].position.x,
                           vertArray[i].position.y,
                           vertArray[i].size,
                           vertArray[i].polygonColour);
            }

            /*
            DrawTriangle(vertArray[i].polyArray[0],
                         vertArray[i].polyArray[1],
                         vertArray[i].polyArray[2],
                         BLUE);
            */
         }

         DrawText(framesPerSecond.c_str(), 10, 40, 20, DARKGRAY);
         DrawText(timeSinceInitialization.c_str(), 10, 10, 20, DARKGRAY);

      EndDrawing();
   //----------------------------------------------------------------------------------
   }

   CloseWindow();

   return 0;
}