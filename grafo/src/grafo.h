#ifndef _GRAFO_H_
#define _GRAFO_H_

#include <ctime>
#include <deque>
#include <random>
#include <algorithm> 
#include <math.h>
#include <stack>
#include <queue>

#include "nodo.h"

using namespace std;

template<class T>
class compare{
public:
    bool operator()(T A, T B){
        return A->obtener_hg() > B->obtener_hg();
    }
};

template <class n, class e>
class CGraph{
    public:
    typedef CGraph<n,e>     self; 
    typedef CNode<self>     Node;
    typedef CEdge<self>     Edge;
    typedef n   N;
    typedef e   E;

    vector<Node *> m_nodes;
    vector<Edge *> m_edges;
    
    

    bool insertNode(N,int,int);
    bool insertEdge(E,N,N,bool);
    void restaurar_estado();
    void generateNode(int);
    void generateEdge();    
    vector<pair<int, int> > armar_ruta(int *,int,int);
    vector<pair<int, int> > depth_search(Node*,Node*);
    vector<pair<int, int> > asterisk_search(Node*,Node*);
    Node *obtener_cercano(int, int);
};



template <class n, class e>
void CGraph<n,e>::generateNode(int cantidad){
    srand(time(NULL));
    int x,y;
    for(int i=0;i<cantidad;i++){
        x=rand()%650;
        y=rand()%650;
        
        insertNode(i,x,y);
    }
}

template <class n, class e>
CNode<CGraph<n,e> >* CGraph<n,e>::obtener_cercano(int x, int y){//del mouse cuando hago click
    float dist,dx,dy;
    float minimo=10000;
    
    int indice;

    for(unsigned i=0;i<m_nodes.size();i++){
        dx = pow(m_nodes[i]->m_punto[0]-x,2);
        dy = pow(m_nodes[i]->m_punto[1] -y, 2);
        dist = sqrt(dx+dy);        

        if(dist<minimo){
            minimo=dist;
            indice=i;
        }
    }
    return m_nodes[indice];
}

template<class n, class e>
void CGraph<n,e>::generateEdge(){
    srand(time(NULL));
    for(unsigned i=0; i<m_nodes.size(); i++){
        vector<pair<float,Node*>> vec;//almaceno distancia y nodo vecino
        for(int j=0; j<m_nodes.size(); j++){
            if(i!=j){
                float dx, dy, dist;
                dx = pow((m_nodes[i]->m_punto[0] - m_nodes[j]->m_punto[0]), 2);
                dy = pow((m_nodes[i]->m_punto[1] - m_nodes[j]->m_punto[1]), 2);
                dist = sqrt(dx+dy);
                vec.push_back(make_pair(dist,m_nodes[j]));
            }
        }

        int numVecinos=rand()%9+2;
        sort(vec.begin(), vec.end());
        

        for(int k=0; k<numVecinos; k++)
        {
            if(numVecinos > m_nodes[i]->m_arista.size()){
                insertEdge(vec[k].first, m_nodes[i]->m_data, vec[k].second->m_data,0);
                
            }            
        }
    }      
}

template<class n, class e>
vector<pair<int, int> > CGraph<n,e>::armar_ruta(int *vino_de, int inicio, int fin){
    int idx;
    vector<pair<int, int> > ruta;
    Node *tmp = m_nodes[fin];

    ruta.push_back(make_pair(tmp->m_punto[0], tmp->m_punto[1]));

    while(tmp->m_data != inicio){
        idx = tmp->m_data;

        // cout << vino_de[idx] << " ";
        tmp = m_nodes[vino_de[idx]];
        ruta.push_back(make_pair(tmp->m_punto[0], tmp->m_punto[1]));
    }

    return ruta;
}

template<class n, class e>
void CGraph<n,e>::restaurar_estado(){
    for(int i=0; i<m_nodes.size(); i++)
        m_nodes[i]->estado = false;
}

template<class n, class e>
vector<pair<int, int> > CGraph<n,e>::depth_search(Node* inicio, Node* fin){
    restaurar_estado();
    vector<pair<int, int> > path;

    stack<Node *> st;
    st.push(inicio);

    Node *current;
    Node *temp;
    int *vino_de = new int[m_nodes.size()];
    int idx;

    while(!st.empty()){
        current = st.top();
        st.pop();

        if(current->m_data == fin->m_data){
            return armar_ruta(vino_de, inicio->m_data, fin->m_data);
        }

        for(int i=0; i<current->m_arista.size(); i++){//entra a aristas para agregar hijos
            temp = current->m_arista[i]->m_nodo[1];
            if(!temp->estado){
                // cout << "push ";
                temp->estado = true;
                st.push(temp);

                idx = temp->m_data;
                vino_de[idx] = current->m_data;
            }
        }
    }

    return path;
}

template<class n, class e>
vector<pair<int, int> > CGraph<n,e>::asterisk_search(Node* inicio, Node* fin){
    restaurar_estado();
    vector<pair<int, int> > path;

    std::priority_queue<Node *, std::vector<Node *>, compare<Node *> > pq;

    Node *current;
    Node *temp;
    pq.push(inicio);

    int *vino_de = new int[m_nodes.size()];

    float h, g;
    int idx;

    while(!pq.empty()){
        current = pq.top();
        pq.pop();

        if(current->m_data == fin->m_data){
            return armar_ruta(vino_de, inicio->m_data, fin->m_data);
        }

        for(int i=0; i<current->m_arista.size(); i++){
            temp = current->m_arista[i]->m_nodo[1];
            if(!temp->estado){
                temp->estado = true;
                
                h = temp->distancia(fin);
                g = current->m_arista[i]->m_data;

                temp->poner_hg(h+g);
                pq.push(temp);

                idx = temp->m_data;
                vino_de[idx] = current->m_data;
            }
        }
    }

    return path;
}

template <class n, class e>
bool CGraph<n,e>::insertNode(N _data, int _x, int _y){
    Node* new_node=new Node(_data,_x,_y);
    m_nodes.push_back(new_node);
    return 1;
}

template <class n, class e>
bool CGraph<n,e>::insertEdge(E _data, N a, N b, bool _direccion){
    Node *Na, *Nb;
    for(int i = 0; i < m_nodes.size(); i++){
        if(m_nodes[i]->m_data==a){
            Na=m_nodes[i];
        }
        if(m_nodes[i]->m_data==b){
            Nb=m_nodes[i];
        }
    }
    // Edge* new_edge=new Edge(_data,Na,Nb,_direccion);
    Na->m_arista.push_back(new Edge(_data,Na,Nb,_direccion));
    Nb->m_arista.push_back(new Edge(_data,Nb,Na,_direccion));
    // m_edges.push_back(new_edge);
    return 1;
}

#endif