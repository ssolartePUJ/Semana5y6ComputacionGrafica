//=============================================================================
// Sample Application: Lighting (Per Fragment Phong)
//=============================================================================

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glApplication.h"
#include "glutWindow.h"
#include <iostream>
#include "glsl.h"
#include <time.h>
#include "glm.h"
#include <FreeImage.h> //*** Para Textura: Incluir librer�a

//-----------------------------------------------------------------------------


class myWindow : public cwc::glutWindow
{
protected:
   cwc::glShaderManager SM;
   cwc::glShader *shader;
   cwc::glShader* shader1; //Para Textura: variable para abrir los shader de textura
   GLuint ProgramObject;
   clock_t time0,time1;
   float timer010;  // timer counting 0->1->0
   bool bUp;        // flag if counting up or down.
   GLMmodel* islandPtr;
   GLMmodel* umbrellaPtr; //*** Para Textura: variable para objeto texturizado
   GLMmodel* boatPtr;
   GLMmodel* discoPtr;
   GLuint grayTexture; //*** Para Textura: variable que almacena el identificador de textura
   GLuint ballTexture;
   GLuint islandTexture;

public:
	myWindow(){}

	//*** Para Textura: aqui adiciono un m�todo que abre la textura en JPG
	void initialize_textures(void)
	{
		ballTexture = initialize_texture("./Mallas/boat.png");
	}

	GLuint initialize_texture(const char* texPath) {
		int w, h;
		GLubyte* data = 0;
		GLuint texId;

		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		FIBITMAP* bitmap = FreeImage_Load(
			FreeImage_GetFileType(texPath, 0),
			texPath);  //*** Para Textura: esta es la ruta en donde se encuentra la textura

		FIBITMAP* pImage = FreeImage_ConvertTo32Bits(bitmap);
		int nWidth = FreeImage_GetWidth(pImage);
		int nHeight = FreeImage_GetHeight(pImage);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight,
			0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage));

		FreeImage_Unload(pImage);

		glEnable(GL_TEXTURE_2D);

		return texId;
	}


	virtual void OnRender(void)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
      //timer010 = 0.09; //for screenshot!

      glPushMatrix();
	  glRotatef(timer010 * 360, 0.5, 1.0f, 0.1f); // Rotation View

	  //glRotatef(90, 1, 0.0f, 0.0f); //Topdown View
	  //glTranslatef(0.0f, 2.0f, 0.0f);


	  /*
      if (shader1) shader1->begin();
		  
		  glPushMatrix();
		  glTranslatef(-1.5f, 0.0f, 0.0f);
		  glBindTexture(GL_TEXTURE_2D, grayTexture);
		  glmDraw(islandPtr, GLM_SMOOTH  | GLM_MATERIAL | GLM_TEXTURE);
		  glPopMatrix();
	      //glutSolidTeapot(1.0);
      if (shader1) shader1->end();

	  //*** Para Textura: llamado al shader para objetos texturizados
	  if (shader1) shader1->begin();
		  glPushMatrix();
		  glTranslatef(1.5f, 0.0f, 0.0f);
		  glBindTexture(GL_TEXTURE_2D, ballTexture);
		  glmDraw(islandPtr, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
		  glPopMatrix();
	  if (shader1) shader1->end();

	  */

	  if (shader) shader->begin();
		  glPushMatrix();
		  glTranslatef(0.0f, 0.0f, 0.0f);
		  glmDraw(islandPtr, GLM_SMOOTH | GLM_MATERIAL);
		  glPopMatrix();
	  if (shader) shader->end();

	  if (shader) shader->begin();
		  glPushMatrix();
		  glTranslatef(0.3f, 0.0f, 0.0f);
		  glScalef(0.1f, 0.1f, 0.1f);
		  glmDraw(umbrellaPtr, GLM_SMOOTH | GLM_MATERIAL);
		  glPopMatrix();
	  if (shader) shader->end();

	  if (shader) shader->begin();
		  glPushMatrix();
		  glTranslatef(-0.15f, 0.0f, 0.15f);
		  glScalef(0.1f, 0.1f, 0.1f);
		  glmDraw(discoPtr, GLM_SMOOTH | GLM_MATERIAL);
		  glPopMatrix();
	  if (shader) shader->end();

	  if (shader1) shader1->begin();
		  glPushMatrix();
		  glTranslatef(0.6f, -0.1f, 0.0f);
		  glScalef(0.2f, 0.2f, 0.2f);
		  glBindTexture(GL_TEXTURE_2D, grayTexture);
		  glmDraw(boatPtr, GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
		  glPopMatrix();
	  if (shader1) shader1->end();

      glutSwapBuffers();
      glPopMatrix();

      UpdateTimer();

		Repaint();
	}

	virtual void OnIdle() {}

	// When OnInit is called, a render context (in this case GLUT-Window) 
	// is already available!
	virtual void OnInit()
	{
		glClearColor(0.5f, 0.5f, 1.0f, 0.0f);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);


		shader = SM.loadfromFile("vertexshader.txt","fragmentshader.txt"); // load (and compile, link) from file
		if (shader==0) 
         std::cout << "Error Loading, compiling or linking shader\n";
      else
      {
         ProgramObject = shader->GetProgramObject();
      }

	 //*** Para Textura: abre los shaders para texturas
		shader1 = SM.loadfromFile("vertexshaderT.txt", "fragmentshaderT.txt"); // load (and compile, link) from file
		if (shader1 == 0)
			std::cout << "Error Loading, compiling or linking shader\n";
		else
		{
			ProgramObject = shader1->GetProgramObject();
		}

      time0 = clock();
      timer010 = 0.0f;
      bUp = true;

	  //Abrir mallas


	  umbrellaPtr = initializeObject(umbrellaPtr , "./Mallas/umbrella.obj"); // Objeto de tribu
	  
	  islandPtr = initializeObject(islandPtr , "./Mallas/islandLegacy.obj"); //Objeto natural

	  boatPtr = initializeObject(boatPtr, "./Mallas/boat.obj"); //Objeto Texturado

	  discoPtr = initializeObject(boatPtr, "./Mallas/disco.obj"); // Objeto Artificial


	   
	 //

 
	  //*** Para Textura: abrir archivo de textura
	  grayTexture = initialize_texture("./Mallas/boat.png");
	  ballTexture = initialize_texture("./Mallas/bola.jpg");
	  islandTexture = initialize_texture("./Mallas/island.jpg");
      DemoLight();
	}

	GLMmodel* initializeObject(GLMmodel* objectPtr, char* path) {

		objectPtr = NULL;

		if (!objectPtr)
		{
			objectPtr = glmReadOBJ(path);
			if (!objectPtr)
				exit(0);

			glmUnitize(objectPtr);
			glmFacetNormals(objectPtr);
			glmVertexNormals(objectPtr, 90.0);

		}
		return objectPtr;
	}




	virtual void OnResize(int w, int h)
   {
      if(h == 0) h = 1;
	   float ratio = 1.0f * (float)w / (float)h;

      glMatrixMode(GL_PROJECTION);
	   glLoadIdentity();
	
	   glViewport(0, 0, w, h);

      gluPerspective(45,ratio,1,100);
	   glMatrixMode(GL_MODELVIEW);
	   glLoadIdentity();
	   gluLookAt(0.0f,0.0f,4.0f, 
		          0.0,0.0,-1.0,
			       0.0f,1.0f,0.0f);
   }
	virtual void OnClose(void){}
	virtual void OnMouseDown(int button, int x, int y) {}    
	virtual void OnMouseUp(int button, int x, int y) {}
	virtual void OnMouseWheel(int nWheelNumber, int nDirection, int x, int y){}

	virtual void OnKeyDown(int nKey, char cAscii)
	{       
		if (cAscii == 27) // 0x1b = ESC
		{
			this->Close(); // Close Window!
		} 
	};

	virtual void OnKeyUp(int nKey, char cAscii)
	{
      if (cAscii == 's')      // s: Shader
         shader->enable();
      else if (cAscii == 'f') // f: Fixed Function
         shader->disable();
	}

   void UpdateTimer()
   {
      time1 = clock();
      float delta = static_cast<float>(static_cast<double>(time1-time0)/static_cast<double>(CLOCKS_PER_SEC));
      delta = delta / 4;
      if (delta > 0.00005f)
      {
         time0 = clock();
         if (bUp)
         {
            timer010 += delta;
            if (timer010>=1.0f) { timer010 = 1.0f; bUp = false;}
         }
         else
         {
            timer010 -= delta;
            if (timer010<=0.0f) { timer010 = 0.0f; bUp = true;}
         }
      }
   }

   void DemoLight(void)
   {
     glEnable(GL_LIGHTING);
     glEnable(GL_LIGHT0);
     glEnable(GL_NORMALIZE);
     
     // Light model parameters:
     // -------------------------------------------
     /*
     GLfloat lmKa[] = {0.0, 0.0, 0.0, 0.0 };
     glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);
     
     glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
     glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);
     
     // -------------------------------------------
     // Spotlight Attenuation
     
     GLfloat spot_direction[] = {1.0, -1.0, -1.0 };
     GLint spot_exponent = 30;
     GLint spot_cutoff = 180;
     
     glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
     glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, spot_exponent);
     glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, spot_cutoff);
    
     GLfloat Kc = 1.0;
     GLfloat Kl = 0.0;
     GLfloat Kq = 0.0;
     
     glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,Kc);
     glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, Kl);
     glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, Kq);
     
     
     // ------------------------------------------- 
     // Lighting parameters:

     GLfloat light_pos[] = {0.0f, 5.0f, 5.0f, 1.0f};
     GLfloat light_Ka[]  = {1.0f, 0.5f, 0.5f, 1.0f};
     GLfloat light_Kd[]  = {1.0f, 0.1f, 0.1f, 1.0f};
     GLfloat light_Ks[]  = {1.0f, 1.0f, 1.0f, 1.0f};

     glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
     glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);
     glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);
     glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);

     // -------------------------------------------
     // Material parameters:

     GLfloat material_Ka[] = {0.5f, 0.0f, 0.0f, 1.0f};
     GLfloat material_Kd[] = {0.4f, 0.4f, 0.5f, 1.0f};
     GLfloat material_Ks[] = {0.8f, 0.8f, 0.0f, 1.0f};
     GLfloat material_Ke[] = {0.1f, 0.0f, 0.0f, 0.0f};
     GLfloat material_Se = 20.0f;

     glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_Ka);
     glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_Kd);
     glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_Ks);
     glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_Ke);
     glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material_Se);
	 */
   }
};

//-----------------------------------------------------------------------------

class myApplication : public cwc::glApplication
{
public:
	virtual void OnInit() {std::cout << "Hello World!\n"; }
};

//-----------------------------------------------------------------------------

int main(void)
{
	myApplication*  pApp = new myApplication;
	myWindow* myWin = new myWindow();

	pApp->run();
	delete pApp;
	return 0;
}

//-----------------------------------------------------------------------------

