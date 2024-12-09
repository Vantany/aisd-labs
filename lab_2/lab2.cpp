#include <iostream>
#include <stdexcept>
#include <ctime>
#include <vector>
#include <algorithm>


void initializeRandomSeed() {
    static bool isSeedInitialized = false;
    if (!isSeedInitialized) {
        srand(static_cast<unsigned>(time(nullptr)));
        isSeedInitialized = true;
    }
}


template <typename T>
class LinkedList {
private:

    struct Node {
        T data;
        Node* next;

        Node(T value, Node* nextNode = nullptr)
            : data(value), next(nextNode) {}
    };



    void copyFrom(const LinkedList& other) {
        if (other.tail == nullptr) return;

        Node* current = other.tail->next;
        do {
            push_tail(current->data);
            current = current->next;
        } while (current != other.tail->next);
    }

public:
    Node* tail;
    size_t size;
    LinkedList() : tail(nullptr), size(0) {}

    LinkedList(const LinkedList& other) : tail(nullptr), size(0) {
        copyFrom(other);
    }

    LinkedList(size_t count, T minValue, T maxValue) : tail(nullptr), size(0) {
        for (size_t i = 0; i < count; ++i) {
            push_tail(minValue + rand() % (maxValue - minValue + 1));
        }
    }

    ~LinkedList() {
        while (size > 0) {
            pop_head();
        }
    }

    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            while (size > 0) {
                pop_head();
            }
            copyFrom(other);
        }
        return *this;
    }

    bool operator==(const LinkedList& other) const {
        if (size != other.size) return false;

        Node* current = tail->next;
        Node* otherCurrent = other.tail->next;
        do {
            if (current->data != otherCurrent->data) return false;
            current = current->next;
            otherCurrent = otherCurrent->next;
        } while (current != tail->next);

        return true;

    }

    bool operator!=(const LinkedList& other) const {
        return !(*this == other);
    }

    void push_tail(const T& value) {
        Node* newNode = new Node(value);
        if (tail == nullptr) {
            newNode->next = newNode;
            tail = newNode;
        } else {
            newNode->next = tail->next;
            tail->next = newNode;
            tail = newNode;
        }
        ++size;
    }

    void push_tail(const LinkedList& other) {
        if (other.tail == nullptr) return;

        Node* current = other.tail->next;
        do {
            push_tail(current->data);
            current = current->next;
        } while (current != other.tail->next);
    }

    void push_head(const T& value) {
        Node* newNode = new Node(value);
        if (tail == nullptr) {
            newNode->next = newNode;
            tail = newNode;
        } else {
            newNode->next = tail->next;
            tail->next = newNode;
        }
        ++size;
    }

    void push_head(const LinkedList& other) {
        if (other.tail == nullptr) return;

        Node* otherHead = other.tail->next;
        Node* temp = otherHead;
        Node* newHead = nullptr;
        Node* newTail = nullptr;

        do {
            Node* newNode = new Node(temp->data);
            if(!newHead) {
                newHead = newNode;
                newTail = newNode;
            } else {
                newTail->next = newNode;
                newTail = newNode;
            }
            temp = temp->next;
        } while (temp != otherHead);
        newTail->next = newHead;
        if(!tail) {
            tail = newTail;
            return;
        }

        newTail->next = tail->next;
        tail->next = newHead;
    }

    void pop_head() {
        if (tail == nullptr) throw std::underflow_error("List is empty!");

        Node* head = tail->next;
        if (tail == head) {
            delete head;
            tail = nullptr;
        } else {
            tail->next = head->next;
            delete head;
        }
        --size;
    }

    void pop_tail() {
        if (tail == nullptr) throw std::underflow_error("List is empty!");

        Node* current = tail->next;
        if (tail == current) {
            delete tail;
            tail = nullptr;
        } else {
            while (current->next != tail) {
                current = current->next;
            }
            current->next = tail->next;
            delete tail;
            tail = current;
        }
        --size;
    }

    void delete_node(const T& value) {
        if (tail == nullptr) return;

        Node* current = tail->next;
        Node* prev = tail;

        size_t count = size;
        do {
            if (current->data == value) {
                if (current == tail) {
                    tail = prev;
                }
                prev->next = current->next;
                delete current;
                current = prev->next;
                --size;
            } else {
                prev = current;
                current = current->next;
            }
            --count;
        } while (count > 0);
    }

    T operator[](size_t index) const {
        if (index >= size) throw std::out_of_range("Index out of range!");

        Node* current = tail->next;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }
        return current->data;
    }

    T& operator[](size_t index) {
        if (index >= size) throw std::out_of_range("Index out of range!");

        Node* current = tail->next;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }
        return current->data;
    }

    friend std::ostream& operator<<(std::ostream& os, const LinkedList& list) {
        if (list.tail == nullptr) {
            return os << "0";
        }

        std::vector<T> digits;
        Node* current = list.tail->next;
        do {
            digits.push_back(current->data);
            current = current->next;
        } while (current != list.tail->next);

        for (auto it = digits.rbegin(); it != digits.rend(); ++it) {
            os << *it;
        }

        return os;
    }

    static LinkedList sum(const LinkedList& a, const LinkedList& b) {
        LinkedList result;
        typename LinkedList::Node* aNode = a.tail ? a.tail->next : nullptr;
        typename LinkedList::Node* bNode = b.tail ? b.tail->next : nullptr;
        int carry = 0;

        bool aDone = (aNode == nullptr);
        bool bDone = (bNode == nullptr);

        while (!aDone || !bDone || carry > 0) {
            int aValue = aDone ? 0 : aNode->data;
            int bValue = bDone ? 0 : bNode->data;
            int sum = aValue + bValue + carry;

            carry = sum / 10;
            result.push_tail(sum % 10);

            if (!aDone) {
                aNode = aNode->next;
                if (aNode == a.tail->next) aDone = true;
            }
            if (!bDone) {
                bNode = bNode->next;
                if (bNode == b.tail->next) bDone = true;
            }
    }

        return result;
    }


    static LinkedList multiply(const LinkedList& a, const LinkedList& b) {
        LinkedList result;
        result.push_tail(0);

        typename LinkedList::Node* bNode = b.tail ? b.tail->next : nullptr;
        int bIndex = 0;

        while (bNode) {
            LinkedList partialResult;
            for (int i = 0; i < bIndex; ++i) {
                partialResult.push_tail(0);
            }

            typename LinkedList::Node* aNode = a.tail ? a.tail->next : nullptr;
            int carry = 0;

            while (aNode) {
                int product = bNode->data * aNode->data + carry;
                carry = product / 10;
                partialResult.push_tail(product % 10);
                aNode = (aNode != a.tail) ? aNode->next : nullptr;
            }

            if (carry > 0) {
                partialResult.push_tail(carry);
            }

            result = LinkedList::sum(result, partialResult);

            bNode = (bNode != b.tail) ? bNode->next : nullptr;
            ++bIndex;
        }

        return result;
    }
};

int main() {
    {
    LinkedList<int> number1;
    number1.push_tail(2);
    number1.push_tail(4);
    number1.push_tail(3);

    LinkedList<int> number2;
    number2.push_tail(5);
    number2.push_tail(6);
    number2.push_tail(4);

    LinkedList<int> result = LinkedList<int>::sum(number1, number2);

    std::cout << "Number 1: " << number1 << std::endl;
    std::cout << "Number 2: " << number2 << std::endl;
    std::cout << "Sum: " << result << std::endl;

    number1.push_head(number2);
    std::cout << number1 << std::endl;
    }

    {
    LinkedList<int> number1;
    number1.push_tail(2);
    number1.push_tail(1);

    LinkedList<int> number2;
    number2.push_tail(2);
    number2.push_tail(1);

    std::cout << (number1==number2) << std::endl;

    LinkedList<int> result = LinkedList<int>::multiply(number1, number2);

    std::cout << "Number 1: " << number1 << std::endl;
    std::cout << "Number 2: " << number2 << std::endl;
    std::cout << "Product: " << result << std::endl;
    }
    return 0;
}
