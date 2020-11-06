#include <iostream>

int input = 3018458;

template<class T>
struct Node
{
    T value;
    Node* next = nullptr;

    Node(Node* parent, T data)
    {
        parent->next = this;
        value = data;
    }
    Node(T data)
    {
        value = data;
    }
    void remove_next()
    {
        if(next != nullptr)
        {
            Node* toDelete = next;
            next = next->next;
            delete toDelete;
        }
    }
};

int main()
{
    {
        int length = input;
        int firstIndex = 1;
        int step = 1;
        while(length > 1)
        {
            step *= 2;
            if(length % 2 == 1)
            {
                firstIndex += step;
            }
            length /= 2;
        }
        std::cout<<"[Part 1] Answer: "<<firstIndex<<std::endl;
    }

    {
        Node<int>* head = new Node<int>(1);
        Node<int>* current = head;
        Node<int>* beforeHalfElf;
        for(int i = 2; i <= input; ++i)
        {
            current = new Node<int>(current, i);
            if(i == input/2)
            {
                beforeHalfElf = current;
            }
            if(i == input)
            {
                current->next = head;
            }
        }

        int n = input;
        while(beforeHalfElf->next != beforeHalfElf)
        {
            beforeHalfElf->remove_next();
            if(n % 2 == 1)
            {
                beforeHalfElf = beforeHalfElf->next;
            }
            --n;
        }
        std::cout<<"[Part 2] Answer: "<<beforeHalfElf->value<<std::endl;
    }
    return 0;
}

/*
     1
   7   2
  6 5 4 3
*/

