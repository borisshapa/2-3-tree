#include <bits/stdc++.h>

#define prev prevvv
using namespace std;

typedef struct Node {
    int size;
    int key[3];
    Node *first;
    Node *second;
    Node *third;
    Node *fourth;
    Node *parent;
} Node;

bool is_leaf(Node *v) {
    return (v->first == nullptr) && (v->second == nullptr) && (v->third == nullptr);
}

bool find_key(Node *v, int x) {
    for (int i = 0; i < v->size; i++)
        if (v->key[i] == x)
            return true;
    return false;
}

Node *find(Node *v, int x) {
    if (v == nullptr)
        return nullptr;
    if (find_key(v, x))
        return v;
    else if (x < v->key[0])
        return find(v->first, x);
    else if ((v->size == 2 && x < v->key[1]) || v->size == 1)
        return find(v->second, x);
    else if (v->size == 2)
        return find(v->third, x);
    return nullptr;
}

Node *split(Node *v) {
    if (v->size < 3)
        return v;
    Node *x = new Node;
    x->size = 1;
    x->key[0] = v->key[0];
    x->key[1] = x->key[2] = 0;
    x->first = v->first;
    x->second = v->second;
    x->third = x->fourth = nullptr;
    x->parent = v->parent;

    Node *y = new Node;
    y->size = 1;
    y->key[0] = v->key[2];
    y->key[1] = y->key[2] = 0;
    y->first = v->third;
    y->second = v->fourth;
    y->third = y->fourth = nullptr;
    y->parent = v->parent;

    if (x->first != nullptr)
        x->first->parent = x;
    if (x->second != nullptr)
        x->second->parent = x;

    if (y->first != nullptr)
        y->first->parent = y;
    if (y->second != nullptr)
        y->second->parent = y;

    if (v->parent != nullptr) {
        v->parent->key[v->parent->size++] = v->key[1];
        sort(v->parent->key, v->parent->key + v->parent->size);

        if (v->parent->first == v)
            v->parent->first = nullptr;
        else if (v->parent->second == v)
            v->parent->second = nullptr;
        else if (v->parent->third == v)
            v->parent->third = nullptr;

        if (v->parent->first == nullptr) {
            v->parent->fourth = v->parent->third;
            v->parent->third = v->parent->second;
            v->parent->second = y;
            v->parent->first = x;
        } else if (v->parent->second == nullptr) {
            v->parent->fourth = v->parent->third;
            v->parent->third = y;
            v->parent->second = x;
        } else {
            v->parent->fourth = y;
            v->parent->third = x;
        }

        Node *u = v->parent;
        delete v;
        return u;
    } else {
        x->parent = v;
        y->parent = v;
        v->key[0] = v->key[1];
        v->first = x;
        v->second = y;
        v->third = nullptr;
        v->fourth = nullptr;
        v->parent = nullptr;
        v->size = 1;
        return v;
    }
}

Node *insert(Node *v, int x) {
    if (v == nullptr) {
        v = new Node;
        v->size = 1;
        v->key[0] = x;
        v->key[1] = v->key[2] = 0;
        v->first = v->second = v->third = v->fourth = v->parent = nullptr;
        return v;
    }
    if (is_leaf(v)) {
        v->key[v->size++] = x;
        sort(v->key, v->key + v->size);
    } else if (x <= v->key[0]) {
        insert(v->first, x);
    } else if (v->size == 1 || (v->size == 2 && x <= v->key[1])) {
        insert(v->second, x);
    } else {
        insert(v->third, x);
    }

    return split(v);
}

Node *find_min(Node *v) {
    if (v == nullptr)
        return v;
    if (v->first == nullptr)
        return v;
    else
        return find_min(v->first);
}

Node *redistribute(Node *v) {
    Node *par = v->parent;
    Node *first = par->first;
    Node *second = par->second;
    Node *third = par->third;
    if (par->size == 2 && first->size < 2 && second->size < 2 && third->size < 2) {
        if (first == v) {
            par->first = par->second;
            par->second = par->third;
            par->third = nullptr;
            par->first->key[par->first->size++] = par->key[0];
            sort(par->first->key, par->first->key + par->first->size);
            par->first->third = par->first->second;
            par->first->second = par->first->first;
            if (v->first != nullptr)
                par->first->first = v->first;
            else if (v->second != nullptr)
                par->first->first = v->second;
            if (par->first->first != nullptr)
                par->first->first->parent = par->first;
            if (par->size >= 1) {
                par->key[0] = par->key[1];
                par->key[1] = par->key[2];
                par->size--;
            } else if (par->size == 2 && par->key[1] == par->key[0]) {
                par->key[1] = par->key[2];
                par->size--;
            }
            delete first;
        } else if (second == v) {
            first->key[first->size++] = par->key[0];
            sort(first->key, first->key + first->size);
            if (par->size >= 1) {
                par->key[0] = par->key[1];
                par->key[1] = par->key[2];
                par->size--;
            } else if (par->size == 2 && par->key[1] == par->key[0]) {
                par->key[1] = par->key[2];
                par->size--;
            }
            if (v->first != nullptr)
                first->third = v->first;
            else if (v->second != nullptr)
                first->third = v->second;
            if (first->third != nullptr)
                first->third->parent = first;
            par->second = par->third;
            par->third = nullptr;
            delete second;
        } else if (third == v) {
            second->key[second->size++] = par->key[1];
            sort(second->key, second->key + second->size);
            par->third = nullptr;
            if (par->size >= 1 && par->key[0] == par->key[1]) {
                par->key[0] = par->key[1];
                par->key[1] = par->key[2];
                par->size--;
            } else if (par->size == 2) {
                par->key[1] = par->key[2];
                par->size--;
            }
            if (v->first != nullptr)
                second->third = v->first;
            else if (v->second != nullptr)
                second->third = v->second;
            if (second->third != nullptr)
                second->third->parent = second;
            delete third;
        }
    } else if (par->size == 2 && (first->size == 2 || second->size == 2 || third->size == 2)) {
        if (third == v) {
            if (v->first != nullptr) {
                v->second = v->first;
                v->first = nullptr;
            }
            v->key[v->size++] = par->key[1];
            sort(v->key, v->key + v->size);
            if (second->size == 2) {
                par->key[1] = second->key[1];
                if (second->size >= 1 && second->key[0] == second->key[1]) {
                    second->key[0] = second->key[1];
                    second->key[1] = second->key[2];
                    second->size--;
                } else if (second->size == 2) {
                    second->key[1] = second->key[2];
                    second->size--;
                }
                v->first = second->third;
                second->third = nullptr;
                if (v->first != nullptr)
                    v->first->parent = v;
            } else if (first->size == 2) {
                par->key[1] = second->key[0];
                v->first = second->second;
                second->second = second->first;
                if (v->first != nullptr)
                    v->first->parent = v;
                second->key[0] = par->key[0];
                par->key[0] = first->key[1];
                if (first->size >= 1 && first->key[0] == first->key[1]) {
                    first->key[0] = first->key[1];
                    first->key[1] = first->key[2];
                    first->size--;
                } else if (first->size == 2) {
                    first->key[1] = first->key[2];
                    first->size--;
                }
                second->first = first->third;
                if (second->first != nullptr)
                    second->first->parent = second;
                first->third = nullptr;
            }
        } else if (second == v) {
            if (third->size == 2) {
                if (v->first == nullptr) {
                    v->first = v->second;
                    v->second = nullptr;
                }
                second->key[second->size++] = par->key[1];
                sort(second->key, second->key + second->size);
                par->key[1] = third->key[0];
                if (third->size >= 1) {
                    third->key[0] = third->key[1];
                    third->key[1] = third->key[2];
                    third->size--;
                } else if (third->size == 2 && third->key[1] == third->key[0]) {
                    third->key[1] = third->key[2];
                    third->size--;
                }
                second->second = third->first;
                if (second->second != nullptr)
                    second->second->parent = second;
                third->first = third->second;
                third->second = third->third;
                third->third = nullptr;
            } else if (first->size == 2) {
                if (v->second == nullptr) {
                    v->second = v->first;
                    v->first = nullptr;
                }
                second->key[second->size++] = par->key[0];
                sort(second->key, second->key + second->size);
                par->key[0] = first->key[1];
                if (first->size >= 1 && first->key[0] == first->key[1]) {
                    first->key[0] = first->key[1];
                    first->key[1] = first->key[2];
                    first->size--;
                } else if (first->size == 2) {
                    first->key[1] = first->key[2];
                    first->size--;
                }
                second->first = first->third;
                if (second->first != nullptr)
                    second->first->parent = second;
                first->third = nullptr;
            }
        } else if (first == v) {
            if (v->first == nullptr) {
                v->first = v->second;
                v->second = nullptr;
            }
            first->key[first->size++] = par->key[0];
            sort(first->key, first->key + first->size);
            if (second->size == 2) {
                par->key[0] = second->key[0];
                if (second->size >= 1) {
                    second->key[0] = second->key[1];
                    second->key[1] = second->key[2];
                    second->size--;
                } else if (second->size == 2 && second->key[1] == second->key[0]) {
                    second->key[1] = second->key[2];
                    second->size--;
                }
                first->second = second->first;
                if (first->second != nullptr)
                    first->second->parent = first;
                second->first = second->second;
                second->second = second->third;
                second->third = nullptr;
            } else if (third->size == 2) {
                par->key[0] = second->key[0];
                second->key[0] = par->key[1];
                par->key[1] = third->key[0];
                if (third->size >= 1) {
                    third->key[0] = third->key[1];
                    third->key[1] = third->key[2];
                    third->size--;
                } else if (third->size == 2 && third->key[1] == third->key[0]) {
                    third->key[1] = third->key[2];
                    third->size--;
                }
                first->second = second->first;
                if (first->second != nullptr)
                    first->second->parent = first;
                second->first = second->second;
                second->second = third->first;
                if (second->second != nullptr)
                    second->second->parent = second;
                third->first = third->second;
                third->second = third->third;
                third->third = nullptr;
            }
        }
    } else if (par->size == 1) {
        v->key[v->size++] = par->key[0];
        sort(v->key, v->key + v->size);
        if (first == v && second->size == 2) {
            par->key[0] = second->key[0];
            if (second->size >= 1) {
                second->key[0] = second->key[1];
                second->key[1] = second->key[2];
                second->size--;
            } else if (second->size == 2 && second->key[1] == second->key[0]) {
                second->key[1] = second->key[2];
                second->size--;
            }
            if (v->first == nullptr)
                v->first = v->second;
            v->second = second->first;
            second->first = second->second;
            second->second = second->third;
            second->third = nullptr;
            if (v->second != nullptr)
                v->second->parent = v;
        } else if (second == v && first->size == 2) {
            par->key[0] = first->key[1];
            if (first->size >= 1 && first->key[0] == first->key[1]) {
                first->key[0] = first->key[1];
                first->key[1] = first->key[2];
                first->size--;
            } else if (first->size == 2) {
                first->key[1] = first->key[2];
                first->size--;
            }
            if (v->second == nullptr)
                v->second = v->first;
            v->first = first->third;
            first->third = nullptr;
            if (v->first != nullptr)
                v->first->parent = v;
        }
    }
    return par;
}

Node *merge(Node *v) {
    Node *par = v->parent;
    if (par->first == v) {
        par->second->key[par->second->size++] = par->key[0];
        sort(par->second->key, par->second->key + par->second->size);
        par->second->third = par->second->second;
        par->second->second = par->second->first;
        if (v->first != nullptr)
            par->second->first = v->first;
        else if (v->second != nullptr)
            par->second->first = v->second;
        if (par->second->first != nullptr)
            par->second->first->parent = par->second;
        if (par->size >= 1) {
            par->key[0] = par->key[1];
            par->key[1] = par->key[2];
            par->size--;
        } else if (par->size == 2 && par->key[1] == par->key[0]) {
            par->key[1] = par->key[2];
            par->size--;
        }
        delete par->first;
        par->first = nullptr;
    } else if (par->second == v) {
        par->first->key[par->first->size++] = par->key[0];
        sort(par->first->key, par->first->key + par->first->size);
        if (v->first != nullptr)
            par->first->third = v->first;
        else if (v->second != nullptr)
            par->first->third = v->second;
        if (par->first->third != nullptr)
            par->first->third->parent = par->first;
        if (par->size >= 1) {
            par->key[0] = par->key[1];
            par->key[1] = par->key[2];
            par->size--;
        } else if (par->size == 2 && par->key[1] == par->key[0]) {
            par->key[1] = par->key[2];
            par->size--;
        }
        delete par->second;
        par->second = nullptr;
    }

    if (par->parent == nullptr) {
        Node *u = nullptr;
        if (par->first != nullptr)
            u = par->first;
        else
            u = par->second;
        u->parent = nullptr;
        delete par;
        return u;
    }
    return par;
}

Node *fix(Node *v) {
    if (v->size == 0 && v->parent == nullptr) {
        delete v;
        return nullptr;
    }
    if (v->size != 0) {
        if (v->parent)
            return fix(v->parent);
        else
            return v;
    }

    Node *par = v->parent;
    if (par->first->size == 2 || par->second->size == 2
        || par->size == 2 || (par->size == 2 && par->third->size == 2)) {
        v = redistribute(v);
    } else {
        v = merge(v);
    }

    return fix(v);
}

Node *del(Node *v, int x) {
    Node *del_node = find(v, x);
    if (del_node == nullptr)
        return v;
    Node *min_node = nullptr;
    if (del_node->key[0] == x)
        min_node = find_min(del_node->second);
    else
        min_node = find_min(del_node->third);

    if (min_node != nullptr) {
        if (x == del_node->key[0])
            swap(del_node->key[0], min_node->key[0]);
        else
            swap(del_node->key[1], min_node->key[0]);
        del_node = min_node;
    }

    if (del_node->size >= 1 && del_node->key[0] == x) {
        del_node->key[0] = del_node->key[1];
        del_node->key[1] = del_node->key[2];
        del_node->size--;
    } else if (del_node->size == 2 && del_node->key[1] == x) {
        del_node->key[1] = del_node->key[2];
        del_node->size--;
    }
    return fix(del_node);
}

int next(Node *v, int x) {
    Node *curr = v;
    int ans = -2000000000;
    while (curr != nullptr) {
        if (curr->key[0] > x) {
            ans = curr->key[0];
            curr = curr->first;
        } else if (curr->size == 2 && x < curr->key[1]) {
            ans = curr->key[1];
            curr = curr->second;
        } else if (curr->size == 1) {
            curr = curr->second;
        } else if (curr->size == 2) {
            curr = curr->third;
        }
    }
    return ans;
}

int prev(Node *v, int x) {
    Node *curr = v;
    int ans = 2000000000;
    while (curr != nullptr) {
        if (curr->size == 2 && curr->key[1] < x) {
            ans = curr->key[1];
            curr = curr->third;
        } else if (curr->key[0] < x) {
            ans = curr->key[0];
            curr = curr->second;
        } else {
            curr = curr->first;
        }
    }
    return ans;
}

void print_tree(Node *v) {
    if (v->first != nullptr)
        print_tree(v->first);
    cout << v->key[0] << ' ';
    if (v->second != nullptr)
        print_tree(v->second);
    if (v->size == 2)
        cout << v->key[1] << ' ';
    if (v->third != nullptr)
        print_tree(v->third);
}

int main() {
#ifdef LOCAL_JOKE
    freopen("input.txt", "r", stdin);
#endif
    ios_base::sync_with_stdio(false);
    string command;
    Node *root = nullptr;
    while (cin >> command) {
        int x;
        cin >> x;
        if (command == "insert") {
            if (find(root, x) == nullptr)
                root = insert(root, x);
        } else if (command == "exists") {
            if (find(root, x) == nullptr)
                cout << "false\n";
            else
                cout << "true\n";
        } else if (command == "delete") {
            if (find(root, x) != nullptr)
                root = del(root, x);
        } else if (command == "next") {
            int ans = next(root, x);
            if (ans == -2000000000)
                cout << "none\n";
            else
                cout << ans << '\n';
        } else if (command == "prev") {
            int ans = prev(root, x);
            if (ans == 2000000000)
                cout << "none\n";
            else
                cout << ans << '\n';
        }
    }
    return 0;
}
