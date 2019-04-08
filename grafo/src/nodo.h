#ifndef _NODO_H_
#define _NODO_H_

#include <vector>

using namespace std;

template <class G>
class CNode{
public:
    typedef typename G::N        N;
    typedef typename G::Edge     Edge;
    typedef typename G::Node     Node;

    N m_data;
    float m_hg;
    vector<Edge *> m_arista;
    int m_punto[2];//0->x  1->y
    bool estado;//si ya se pasó por este nodo, su estado cambia a 1 y no vuelve a pasar
    CNode(N _data,int x,int y){
        m_data=_data;
        m_punto[0]=x;
        m_punto[1]=y;
        estado=0;
    }

    void poner_hg(float _hg){
        this->m_hg = _hg;
    }

    float obtener_hg(){
        return this->m_hg;
    }

    float distancia(Node *tmp){
        float dist = (float)(sqrt(pow(m_punto[0]-tmp->m_punto[0],2) + pow(m_punto[1]-tmp->m_punto[1],2)));
        return dist;
    }
};

template <class G>
class CEdge{
public:
    typedef typename G::Node    Node;
    typedef typename G::E       E;
    
    bool m_direccion;//0 -> bidireccional
    E m_data;
    Node* m_nodo[2];//nodo[0] el nodo inicio, nodo[1]  nodo fin

    CEdge(E _data, Node *inicio, Node *fin, bool _direccion){
        m_data=_data;
        m_direccion=_direccion;
        m_nodo[0]= inicio;
        m_nodo[1]= fin;
    }
};

#endif