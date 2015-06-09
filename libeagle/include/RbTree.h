/**
 *       Filename:  RbTree.h
 *
 *    Description:  red black tree
 *
 *        Version:  1.0
 *        Created:  05/05/2015 09:45:53 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Chen, 228268157@qq.com
 *   Organization:  
 */
#ifndef  _RbTree_H_
#define  _RbTree_H_

template<class K, class V> class RbTree
{
public:
#define SET_RED(n) (n)->color = RED
#define SET_BLACK(n) (n)->color = BLACK

#define IS_BLACK(n) (NULL == (n) || BLACK == (n)->color)
#define IS_RED(n) (NULL != n && RED == (n)->color)

#define REPLACE(d, s) (d)->key = s->key;\
    (d)->val = s->val

    struct Pair
    {
        const K key;
        V val;
    };

    class Iterator
    {
    public:
        Iterator() : m_node(NULL){}

        Iterator operator++()
        {
            if (NULL == m_node) return *this;

            m_node = m_tree->getNext(m_node);

            return *this;
        }

        Iterator operator--()
        {
            if (NULL == m_node) return *this;

            m_node = m_tree->getPrev(m_node);

            return *this;
        }

        Pair *operator->() const
        {
            return (Pair *)m_node;
        }

    private:
        Iterator(typename RbTree<K, V>::Node *n, RbTree<K, V> *tree) 
            : m_node(n), m_tree(tree){}

        typename RbTree<K, V>::Node *m_node;
        RbTree<K, V> *m_tree;

        friend class RbTree<K, V>;
        friend bool operator== (const typename RbTree<K, V>::Iterator &l, 
                const typename RbTree<K, V>::Iterator &r)
        {
            return l.m_node == r.m_node;
        }

        friend bool operator!= (const typename RbTree<K, V>::Iterator &l, 
                const typename RbTree<K, V>::Iterator &r)
        {
            return l.m_node != r.m_node;
        }
    };

    RbTree()
    {
        m_root = NULL;
        m_max = NULL;
        m_min = NULL;
        m_elements = 0;
        m_iterListHead = NULL;
    }

    ~RbTree()
    {
        
    }
    
    Iterator begin()
    {
        return Iterator(m_iterListHead, this);
    }

    Iterator end()
    {
        static Iterator iter(NULL, this);

        return iter;
    }

    Iterator getMin()
    {
        return Iterator(m_min, this);
    }

    Iterator getMax()
    {
        return Iterator(m_max, this);
    } 

    int insert(const K &key, const V &val)
    {
        Node *n;

        if (NULL == m_root)
        {
            n = insertRoot(key, val);
        }else
        {
            n = insertChild(key, val);
            if (NULL == n) return -1;
        }

        ++m_elements;
        n->itNext = m_iterListHead;
        if (m_root != n)
        {
            m_iterListHead->itPrev = n;
        }
        m_iterListHead = n;
        if (m_max == NULL || m_max->key < n->key)
        {
            m_max = n;
        }
        if (m_min == NULL || m_min->key > n->key)
        {
            m_min = n;
        }

        return 0;
    }

    int erase(const K &key)
    {
        Node *n = findNode(key);

        if (NULL == n) return -1;

        erase(n);

        return 0;
    }

    Iterator erase(Iterator iter)
    {
        if (NULL == iter.m_node) return Iterator(NULL, this);

        Node *n = iter.m_node->itPrev;

        erase(iter.m_node);
        n = NULL == n ? m_iterListHead : n->itNext;

        return Iterator(n, this);
    } 

    void clear()
    {
        m_elements = 0;
        m_root = NULL;
        m_min = NULL;
        m_max = NULL;
        for(Node *tmp = m_iterListHead; tmp != NULL; )
        {
            m_iterListHead = m_iterListHead->itNext;
            delete tmp; 
            tmp = m_iterListHead;
        }
    }

    Iterator find(const K &key)
    {
        return Iterator(findNode(key), this);
    }

    inline int size()
    {
        return m_elements;
    }

    inline int empty()
    {
        return 0 == m_elements;
    }

private:  
    enum Color
    {
        RED,
        BLACK
    };

    struct Node
    {
        K key;
        V val;
        Node *parent;
        Node *left;
        Node *right; 
        Node *itNext; 
        Node *itPrev; 
        Color color;  

        Node(const K &k, const V &v) : key(k), val(v)
        {
            parent = NULL;
            left = NULL;
            right = NULL;
            itNext = NULL;
            itPrev = NULL;
        }
    };

    int compare(const K &k1, const K &k2)
    {
        if (k1 == k2) return 0;
        if (k1 > k2) return 1;
        return -1;
    }

    Node *getNext(Node *n)
    {
        return n->itNext;
    }

    Node *getPrev(Node *n)
    {
        return n->itPrev;
    }

    Node *insertRoot(const K &key, const V &val)
    {
       Node *n = new Node(key, val); 
       SET_BLACK(n);
       m_root = n;

       return n;
    }

    Node *insertChild(const K &key, const V &val)
    {
        Node *n;
        Node *tmp;
        Node *parent;
        int rs;

        tmp = m_root;
        do
        {
            rs = compare(key, tmp->key);
            if (0 == rs) return NULL;

            parent = tmp;
            tmp = rs > 0 ? tmp->right : tmp->left;
        }while (tmp);

        n = new Node(key, val);
        SET_RED(n); 
        if (rs > 0) 
        {
            parent->right = n;
        }else
        {
            parent->left = n;
        }
        n->parent = parent;

        if (IS_RED(parent))
        {
            insertFixup(n); 
        }

        return n;
    }

    void erase(Node *n)
    {
        n = deleteNode(n);
        if (n->itPrev)
        {
            n->itPrev->itNext = n->itNext;
        }else
        {
            m_iterListHead = n->itNext;
        }

        if (n->itNext)
        {
            n->itNext->itPrev = n->itPrev;
        }
        if (m_max == n)
        {
            m_max = n->left == NULL ? n->parent : n->left;
        }
        if (m_min == n)
        {
            m_min = n->right == NULL ? n->parent : n->right;
        }
        if (0 == m_elements) m_root = NULL;
        delete n;
    }

    Node *deleteNode(Node *n)
    {
        --m_elements;
        if (0 == m_elements) return n;

        Node *del;
        Node *parent;

        if (NULL == n->right)
        {
            del = NULL == n->left ? n : n->left;
        }else
        {
            for (del = n->right; NULL != del->left; del = del->left);
        }

        if (del != n) REPLACE(n, del);
        parent = del->parent; 

        if (IS_BLACK(del))
        {
            if (NULL != del->right)
            {
                del->right->parent = parent;
                SET_BLACK(del->right);
            }else
            {
                deleteFixup(del);
            }
        }
        if (parent->left == del)
        {
            parent->left = del->right;
        }else
        {
            parent->right = del->right;
        }

        return del;
    }

    Node *findNode(const K &key)
    {
        Node *tmp;

        tmp = m_root;
        while (tmp)
        {
            if (key == tmp->key) break;

            tmp = key > tmp->key ? tmp->right : tmp->left;
        }

        return tmp;
    }

    void leftRotate(Node *n)
    {
        Node *tmp;
        Node *parent;

        tmp = n->right;
        if (NULL == tmp) return;

        n->right = tmp->left;
        if (n->right)
        {
            n->right->parent = n;
        }
        tmp->left = n;
        parent = n->parent;  
        tmp->parent = parent;
        n->parent = tmp;
        if (NULL == parent)
        {
            m_root = tmp;

            return;
        }

        if (parent->right == n)
        {
            parent->right = tmp;
        }else
        {
            parent->left = tmp;
        }
    }

    void rightRotate(Node *n)
    {
        Node *tmp;
        Node *parent;

        tmp = n->left;
        if (NULL == tmp) return;

        n->left = tmp->right;
        if (n->left)
        {
            n->left->parent = n;
        }
        tmp->right = n;
        parent = n->parent;  
        tmp->parent = parent;
        n->parent = tmp;
        if (NULL == parent)
        {
            m_root = tmp;

            return;
        }

        if (parent->right == n)
        {
            parent->right = tmp;
        }else
        {
            parent->left = tmp;
        }
    }

    void deleteFixup(Node *n)
    {
        Node *brother;
        Node *parent;

        do
        {
            parent = n->parent; 
            if (n == parent->left)
            {
                brother = parent->right;
                if (IS_RED(brother))
                {
                    SET_BLACK(brother);
                    SET_RED(parent);
                    leftRotate(parent);
                    brother = parent->right;
                }
                if (IS_BLACK(brother->left) && IS_BLACK(brother->right))
                {
                    SET_RED(brother);
                    n = parent;
                }else
                {
                    if (IS_BLACK(brother->right))
                    {
                        SET_BLACK(brother->left); 
                        SET_RED(brother);
                        rightRotate(brother);
                        brother = parent->right;
                    }
                    brother->color = parent->color;
                    SET_BLACK(parent);
                    SET_BLACK(brother->right);
                    leftRotate(parent);

                    return;
                }
            }else
            {
                brother = parent->left;
                if (IS_RED(brother))
                {
                    SET_BLACK(brother);
                    SET_RED(parent);
                    rightRotate(parent);
                    brother = parent->left;
                }
                if (IS_BLACK(brother->left) && IS_BLACK(brother->right))
                {
                    SET_RED(brother);
                    n = parent;
                }else
                {
                    if (IS_BLACK(brother->left))
                    {
                        SET_BLACK(brother->right); 
                        SET_RED(brother);
                        leftRotate(brother);
                        brother = parent->left;
                    }
                    brother->color = parent->color;
                    SET_BLACK(parent);
                    SET_BLACK(brother->left);
                    rightRotate(parent);

                    return;
                } 
            }
        }while (IS_BLACK(n) && n != m_root);
        SET_BLACK(n);
    }

    void insertFixup(Node *n)
    {
        Node *tmp;
        Node *parent;

        parent = n->parent;
        do
        {
            if (parent == parent->parent->left)
            {
                tmp = parent->parent->right;        
                if (IS_RED(tmp))
                {
                    SET_BLACK(tmp);
                    SET_BLACK(parent);
                    n = parent->parent;
                    if (n == m_root) return;

                    SET_RED(n);
                }else
                {
                    if (n == parent->right)
                    {
                        leftRotate(parent);
                        SET_BLACK(parent);
                    }else
                    {
                        SET_BLACK(n);
                        n = parent;
                    }
                    rightRotate(n->parent);
                }
                parent = n->parent;
            }else
            {
                tmp = parent->parent->left; 
                if (IS_RED(tmp))
                {
                    SET_BLACK(tmp);
                    SET_BLACK(parent);
                    n = parent->parent;
                    if (n == m_root) return;

                    SET_RED(n);
                }else
                {
                    if (n == parent->left)
                    {
                        rightRotate(parent);
                        SET_BLACK(parent);
                    }else
                    {
                        SET_BLACK(n);
                        n = parent;
                    }
                    leftRotate(n->parent);
                }
                parent = n->parent;
            }
        }while (IS_RED(parent));

        if (NULL == parent) SET_BLACK(n);
    }

    int m_elements;
    Node *m_root;
    Node *m_max;
    Node *m_min;
    Node *m_iterListHead;
};

#endif   /* ----- #ifndef _RbTree_H_  ----- */
