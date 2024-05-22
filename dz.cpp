#include <iostream>
#include <fstream>
#include <initializer_list>
using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int value) : data(value), left(nullptr), right(nullptr) {}
};

class BSTree {
private:
    Node* root;

public:
    BSTree() : root(nullptr) {}
    BSTree(
        initializer_list<int> list) : root(nullptr) {
        for (int value : list) {
            add_element(value);
        } 
    }

    bool add_element(int value) {
        if (root == nullptr) {
            root = new Node(value);
            return true;
        }

        Node* current = root;
        while (true) {
            if (value < current->data) {
                if (current->left == nullptr) {
                    current->left = new Node(value);
                    return true;
                }
                current = current->left;
            }
            else if (value > current->data) {
                if (current->right == nullptr) {
                    current->right = new Node(value);
                    return true;
                }
                current = current->right;
            }
            else {
                // Элемент уже существует в дереве
                return false;
            }
        }
    }

    bool delete_element(int value) {
        if (root == nullptr) {
            return false;
        }

        Node* parent = nullptr;
        Node* current = root;
        bool is_left_child = false;

        // Поиск удаляемого элемента
        while (current != nullptr && current->data != value) {
            parent = current;
            if (value < current->data) {
                current = current->left;
                is_left_child = true;
            }
            else {
                current = current->right;
                is_left_child = false;
            }
        }

        if (current == nullptr) {
            // Элемент не найден в дереве
            return false;
        }

        // Удаление элемента
        if (current->left == nullptr && current->right == nullptr) {
            // Удаляемый элемент является листом
            if (current == root) {
                root = nullptr;
            }
            else if (is_left_child) {
                parent->left = nullptr;
            }
            else {
                parent->right = nullptr;
            }
            delete current;
        }
        else if (current->right == nullptr) {
            // Удаляемый элемент имеет только левого потомка
            if (current == root) {
                root = current->left;
            }
            else if (is_left_child) {
                parent->left = current->left;
            }
            else {
                parent->right = current->left;
            }
            delete current;
        }
        else if (current->left == nullptr) {
            // Удаляемый элемент имеет только правого потомка
            if (current == root) {
                root = current->right;
            }
            else if (is_left_child) {
                parent->left = current->right;
            }
            else {
                parent->right = current->right;
            }
            delete current;
        }
        else {
            // Удаляемый элемент имеет обоих потомков
            Node* successor = get_successor(current);
            if (current == root) {
                root = successor;
            }
            else if (is_left_child) {
                parent->left = successor;
            }
            else {
                parent->right = successor;
            }
            successor->left = current->left;
            delete current;
        }

        return true;
    }

    Node* get_successor(Node* node) {
        Node* successor_parent = node;
        Node* successor = node;
        Node* current = node->right;

        while (current != nullptr) {
            successor_parent = successor;
            successor = current;
            current = current->left;
        }

        if (successor != node->right) {
            successor_parent->left = successor->right;
            successor->right = node->right;
        }

        return successor;
    }

    bool find_element(int value) {
        Node* current = root;

        while (current != nullptr) {
            if (value < current->data) {
                current = current->left;
            }
            else if (value > current->data) {
                current = current->right;
            }
            else {
                return true;  // Элемент найден
            }
        }

        return false;  // Элемент не найден
    }

    void print() {
        print_tree(root);
    }

    void print_tree(Node* node) {
        if (node != nullptr) {
            print_tree(node->left);
            cout << node->data << " ";
            print_tree(node->right);
        }
    }

    bool save_to_file(const string& path) {
        ofstream file(path);
        if (!file.is_open()) {
            return false;  // Не удалось открыть файл для записи
        }

        save_tree(root, file);

        file.close();
        return true;  // Дерево успешно сохранено в файл
    }

    void save_tree(Node* node, ofstream& file) {
        if (node != nullptr) {
            file << node->data << endl;
            save_tree(node->left, file);
            save_tree(node->right, file);
        }
    }

    bool load_from_file(const string& path) {
        ifstream file(path);
        if (!file.is_open()) {
            return false;  // Не удалось открыть файл для чтения
        }

        clear_tree(root);  // Очистка текущего дерева

        int value;
        while (file >> value) {
            add_element(value);
        }

        file.close();
        return true;  // Дерево успешно загружено из файла
    }

    void clear_tree(Node* node) {
        if (node != nullptr) {
            clear_tree(node->left);
            clear_tree(node->right);
            delete node;
        }
    }

    ~BSTree() {
        clear_tree(root);
    }
};

int main() {
    setlocale(LC_ALL, "Rus");
    BSTree tree({ 6,3,8,2,4,7,9,1 });
    tree.add_element(75);
    tree.delete_element(3);
    int t, test1;
    t = 5;
    while (t != 0) {
        cout << "Выберите команду: 1 - добавить элемент, 2 - удалить элемент, 3 - найти элемент, 4 - напечатать дерево, 0 - выход. Ваш выбор: ";
        cin >> t;
        cout << endl;

        if (t == 1) {
            cout << "Введите элемент: ";
            cin >> test1;
            cout << endl;
            tree.add_element(test1);
        }

        if (t == 2) {
            cout << "Введите элемент: ";
            cin >> test1;
            cout << endl;
            tree.delete_element(test1);
        }

        if (t == 3) {
            cout << "Введите элемент: ";
            cin >> test1;
            cout << endl;
            cout << tree.find_element(test1) << endl;
            cout << endl;
        }

        if (t == 4) { tree.print(); cout << endl; }
        if (t == 0) break;
    }
    return 0;
}