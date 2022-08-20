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
   Vector2 polyArray2[3];
   Color polygonColour = RED;

   bool locked = false;
   bool visible = true;

   int id = 0;

   float physicsSpeed = 150.f;
   float gravity = 1.f;
   float pushRadius = 100.f;
   float mass = 1.f;
   float size = 10.f;

   void CreateVertice();
   void Update();
   void Flush();
private:
   Vector2 frameVelocity;

   int polygonIDArray[3] = { 0, 0, 0 };
   int connectedNum;
};

std::vector<WaterVerticeClass> vertArray(0);


__forceinline float GetDistance(Vector2 startPoint, Vector2 endPoint) {
   return sqrt(pow((endPoint.x - startPoint.x), 2) + pow((endPoint.y - startPoint.y), 2));
}

__forceinline float Map(float val, float in1, float in2, float out1, float out2) {
   return out1 + (val - in1) * (out2 - out1) / (in2 - in1);
}

__forceinline Vector2 GetAngle(Vector2 startAngle, Vector2 endAngle) {
   float radiansAngle = atan2(endAngle.y - startAngle.y, endAngle.x - startAngle.x);
   Vector2 vectorAngle = Vector2{(float)cos(radiansAngle), (float)sin(radiansAngle)};

   return vectorAngle;
}



void WaterVerticeClass::CreateVertice() {
   id = vertArray.size() + 1;
   polyArray[0] = position;
   polygonIDArray[0] = id;
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
      WaterVerticeClass indexBall = vertArray[i];
      float distance = GetDistance(position, indexBall.position);
      if(distance >= vertArray[i].pushRadius + pushRadius || indexBall.id == id)
         continue;

      Vector2 vertAngle = GetAngle(position, vertArray[i].position);

      float x = (vertArray[i].pushRadius + pushRadius - distance) * vertAngle.x * -1;
      float y = (vertArray[i].pushRadius + pushRadius - distance) * vertAngle.y * -1;

      if(vertArray[i].locked == false) {
         x = x / 2;
         y = y / 2;
      }

      position = Vector2{position.x + x, position.y + y};

      if(vertArray[i].locked == false) {
         vertArray[i].position.x -= x;
         vertArray[i].position.y -= y;
      }

      /*
      if(distance < vertArray[i].pushRadius) {
         frameVelocity.x += (distance - vertArray[i].pushRadius) * vertAngle.x / 30;
         frameVelocity.y += (distance - vertArray[i].pushRadius) * vertAngle.y / 30;
      }
      */
   }


   //velocity.x += 4.f / (GetDistance(position, Vector2{0, position.y}) + 1);
   //velocity.x -= 4.f / (GetDistance(position, Vector2{(float)GetScreenWidth(), position.y}) + 1);
   float screenHeight = (float)GetScreenHeight();
   float screenWidth = (float)GetScreenWidth();
   mass = 0.001f * (GetDistance(position, Vector2{position.x, screenHeight}));
   
   frameVelocity.y -= 100.f / (screenHeight + 50 - position.y + 1);
   position.y = position.y + (frameVelocity.y + (velocity.y / 100)) * deltaTime * 150;


   //velocity = Vector2{velocity.x - velocity.x / 100, velocity.y - velocity.y / 100};
   position.y = position.y + gravity * deltaTime * mass * physicsSpeed;

   if(position.x > screenWidth)
      position.x = screenWidth;

   if(position.x < 0)
      position.x = 0;

   if(position.y > screenHeight) {
      position.y = screenHeight;
      velocity = Vector2{velocity.x, velocity.y -5.f};
   }
   
   if(position.y < 0)
      position.y = 0;

   /*
   polyArray2[0] = position;
   polyArray2[1] = Vector2{0, 0}; polyArray2[2] = Vector2{0, 0};

   polygonIDArray[0] = id;
   polygonIDArray[1] = 0; polygonIDArray[2] = 0;
   for(int j = 1; j <= 2; j++) {
      for(unsigned int i = 0; i < vertArray.size(); i++) {
         float distance = GetDistance(position, vertArray[i].position);
         int ignoreID = 0;

         if(shortestDistance < shortestDistance2)
            ignoreID = polygonIDArray[2];
         else ignoreID = polygonIDArray[1];
            

         shortestDistance = 99999.f;
         shortestDistance2 = 99999.f;
         

         if(j == 1 && distance < shortestDistance && vertArray[i].id != id && distance < 200 && vertArray[i].polygonIDArray[j] != id && vertArray[i].id != ignoreID) {
            firstId = vertArray[i].id;
            shortestDistance = distance;
            polygonIDArray[j] = id;
            polyArray2[j] = vertArray[i].position;
         }

         if(j == 2 && distance < shortestDistance2 && distance < 200 && vertArray[i].id != firstId && vertArray[i].id != id  && vertArray[i].polygonIDArray[j] != id && vertArray[i].id != ignoreID) {
            shortestDistance2 = distance;
            polygonIDArray[j] = id;
            polyArray2[j] = vertArray[i].position;
         }
      }
   }
   */
   
}

void WaterVerticeClass::Flush() {
   
}

int main(void)
{
   // Variables
   //--------------------------------------------------------------------------------------
   const int screenWidth = 1000;
   const int screenHeight = 540;

   string framesPerSecond;
   string timeSinceInitialization;

   SetConfigFlags(FLAG_WINDOW_RESIZABLE);
   // SetConfigFlags(FLAG_MSAA_4X_HINT);
   
   InitWindow(screenWidth, screenHeight, "Fluid :D");
   rlDisableBackfaceCulling();
   SetTargetFPS(144);

   WaterVerticeClass vertice;
   
   
   vertice.pushRadius = 40.f;
   vertice.polygonColour = BLACK;
   vertice.size = 40.f;
   vertice.locked = true;
   vertice.visible = true;
   vertice.position = Vector2{((float)GetScreenWidth() / 2), ((float)GetScreenHeight() / 2)};
   vertice.CreateVertice();

   vertice.locked = false;
   vertice.size = 2.f;
   vertice.pushRadius = vertice.size + 3.f;
   vertice.polygonColour = RED;
   
   for(unsigned int i = 0; i < 2500; i++) {
      vertice.position = Vector2{((float)GetScreenWidth() / 2) + (i / 10), ((float)GetScreenHeight() / 2) + (i / 10)};
      vertice.CreateVertice();
   }
   

   while (!WindowShouldClose())
   {
      timeSinceInitialization = std::to_string((int)GetTime());
      framesPerSecond = std::to_string(GetFPS());


      for(unsigned int i = 0; i < vertArray.size(); i++)
         vertArray[i].Update();

      vertArray[0].pushRadius = 40.f;

      if(IsMouseButtonPressed(0))
         vertArray[0].pushRadius = 200.f;

      vertArray[0].position = GetMousePosition();

      // Rendering
      //----------------------------------------------------------------------------------
      BeginDrawing();

         ClearBackground(Color{0, 0, 9});

         for(unsigned int i = 0; i < vertArray.size(); i++)
            if(vertArray[i].visible) {
               DrawCircle( vertArray[i].position.x,
                           vertArray[i].position.y,
                           vertArray[i].size,
                           vertArray[i].polygonColour);
            }

         DrawText(framesPerSecond.c_str(), 10, 40, 20, DARKGRAY);
         DrawText(timeSinceInitialization.c_str(), 10, 10, 20, DARKGRAY);
         DrawText(std::string(std::to_string(vertArray.size() - 1) + " Circles").c_str(), GetScreenWidth() - 130, 10, 20, DARKGRAY);

      EndDrawing();
   //----------------------------------------------------------------------------------
   }

   CloseWindow();

   return 0;
}