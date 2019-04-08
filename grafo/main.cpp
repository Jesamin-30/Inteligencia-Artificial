#include <iostream>
#include <vector>
#include <GL/glut.h>

#include "src/grafo.h"

#define KEY_ESC 27
/*
1)genera 500 puntos aleatorios
2)generar vecinos para cada nodo, entre 2 y 10, sus vecinos deben ser los más cercanos
*/

using namespace std;


CGraph<int,int> grafito;
CNode<CGraph<int,int> >* inicio;
CNode<CGraph<int,int> >* fin;
bool inicio_estado, fin_estado = false;
vector<pair<int, int> > profundidad;
vector<pair<int, int> > asterisco;

//dibuja un simple gizmo
void displayGizmo(){
    
    glPointSize(1);
    glBegin(GL_POINTS);

    glColor3f(1,1,1);
        for(unsigned i=0;i<grafito.m_nodes.size();i++){
            glVertex2i(grafito.m_nodes[i]->m_punto[0],grafito.m_nodes[i]->m_punto[1]);      
        }
    glEnd();

    glBegin(GL_LINES);
    glColor3f(1,0,0);
        for(unsigned i = 0; i < grafito.m_nodes.size(); i++){
            for(unsigned j=0;j<grafito.m_nodes[i]->m_arista.size();j++){
                glVertex2f(grafito.m_nodes[i]->m_punto[0],grafito.m_nodes[i]->m_punto[1]);
                glVertex2f(grafito.m_nodes[i]->m_arista[j]->m_nodo[1]->m_punto[0],grafito.m_nodes[i]->m_arista[j]->m_nodo[1]->m_punto[1]);
            }
        }
    glEnd();

    if(inicio_estado){
        glPointSize(6);
        glBegin(GL_POINTS);
        glColor3f(1,1,0);
            glVertex2i(inicio->m_punto[0],inicio->m_punto[1]);
        glEnd();
    }

    if(fin_estado){
        glPointSize(6);
        glBegin(GL_POINTS);
        glColor3f(0,1,1);
            glVertex2i(fin->m_punto[0],fin->m_punto[1]);
        glEnd();
    }

    glBegin(GL_LINES);
    glColor3f(0,1,0);
        for(unsigned i = 0; i < profundidad.size()-1 && profundidad.size()>2; i++){
            glVertex2f(profundidad[i].first, profundidad[i].second);
            glVertex2f(profundidad[i+1].first, profundidad[i+1].second);
        }
    glEnd();

    glBegin(GL_LINES);
    glColor3f(0,0,1);
        for(unsigned i = 0; i < asterisco.size()-1 && asterisco.size()>2; i++){
            glVertex2f(asterisco[i].first, asterisco[i].second);
            glVertex2f(asterisco[i+1].first, asterisco[i+1].second);
        }
    glEnd();
}


void OnMouseClick(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    inicio = grafito.obtener_cercano(x,650-y);
    inicio_estado = true;
    cout<<"mouse: "<<x<<"  "<<y<<endl;
    // cout<<"inicio grafo: "<<inicio->m_punto[0]<<"  "<<inicio->m_punto[1]<<endl;
  }

  if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
  {
    fin = grafito.obtener_cercano(x,650-y);
    fin_estado = true;
    cout<<"mouse: "<<x<<"  "<<y<<endl;
    // cout<<"inicio grafo: "<<fin->m_punto[0]<<"  "<<fin->m_punto[1]<<endl;
  }
}

void OnMouseMotion(int x, int y)
{
     //opcional 
     //hacer algo x,z cuando se mueve el mouse
}



void idle(){ // AGREGAR ESTA FUNCION
    glutPostRedisplay();
}

//funcion llamada a cada imagen
void glPaint(void) {
    //El fondo de la escena al color initial
    glClear(GL_COLOR_BUFFER_BIT); //CAMBIO
    glLoadIdentity();
    //glOrtho(-300.0f,  300.0f,-300.0f, 300.0f, -1.0f, 1.0f);
    glOrtho(-0.0f, 650.0f,-0.0f, 650.0f, -1.0f, 1.0f);
    
    //dibuja el gizmo
    displayGizmo();

    //doble buffer, mantener esta instruccion al fin de la funcion
    glutSwapBuffers();
}

//
//inicializacion de OpenGL
//
void init_GL(void) {
    //Color del fondo de la escena
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro

    //modo projeccion
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}

//en el caso que la ventana cambie de tama�o
GLvoid window_redraw(GLsizei width, GLsizei height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}

GLvoid window_key(unsigned char key, int x, int y) {
    switch (key) {
        case KEY_ESC:
            exit(0);
            break;
        case 'p':{
            profundidad = grafito.depth_search(inicio, fin);
            cout << profundidad.size() << "\n";
            break;
        }
        case 'a':{
            asterisco = grafito.asterisk_search(inicio, fin);
            cout << asterisco.size() << "\n";
            break;
        }
        default:
            break;
    }

}



int main(int argc, char**argv)
{

    grafito.generateNode(500);
    grafito.generateEdge();

    //Inicializacion de la GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(650, 650); //tama�o de la ventana
    glutInitWindowPosition(20, 20); //posicion de la ventana
    glutCreateWindow("GRAFO"); //titulo de la ventana

    init_GL(); //funcion de inicializacion de OpenGL

    glutDisplayFunc(glPaint);
    glutReshapeFunc(&window_redraw);
    // Callback del teclado
    glutKeyboardFunc(&window_key);
    glutMouseFunc(&OnMouseClick);
    glutMotionFunc(&OnMouseMotion);
    glutIdleFunc(&idle);
    glutMainLoop(); //bucle de rendering
    //no escribir nada abajo de mainloop    
    return 0;
}
