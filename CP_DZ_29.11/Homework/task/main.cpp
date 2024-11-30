#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
using namespace std;


class FileProcessResult abstract
{
public:
    virtual void get_message() = 0;
};

template<typename T>
class FileInterface abstract
{
public:
    virtual FileProcessResult* open(ios_base::openmode mode) = 0;
    virtual FileProcessResult* close() = 0;
    virtual FileProcessResult* write(T data, ios_base::openmode mode) = 0;
    virtual FileProcessResult* read(T& data, ios_base::openmode mode) = 0;
};

class FileOpenSuccess : public FileProcessResult
{
public:
    FileOpenSuccess() = default;
    void get_message() override
    {
        cout << "File opened successfully" << endl;
    }
};

class FileOpenError : public FileProcessResult
{
public:
    FileOpenError() = default;
    void get_message() override
    {
        cout << "Error opening file" << endl;
    }
};

class FileCloseSuccess : public FileProcessResult
{
public:
    FileCloseSuccess() = default;
    void get_message() override
    {
        cout << "File closed successfully" << endl;
    }
};

class FileWriteSuccess : public FileProcessResult
{
public:
    FileWriteSuccess() = default;
    void get_message() override
    {
        cout << "Data written successfully" << endl;
    }
};

class FileWriteError : public FileProcessResult
{
    exception e;
public:
    FileWriteError() = default;
    FileWriteError(exception e)
    {
        this->e = e;
    }
    void get_message() override
    {
        cout << "Error writing data" << endl;
    }
};

class FileReadSuccess : public FileProcessResult
{
public:
    FileReadSuccess() = default;
    void get_message() override
    {
        cout << "Data read successfully" << endl;
    }
};

class FileReadError : public FileProcessResult
{
    exception e;
public:
    FileReadError() = default;
    FileReadError(exception e)
    {
        this->e = e;
    }
    void get_message() override
    {
        cout << "Error writing data" << endl;
    }
};

template<typename T>
class FileService : public FileInterface<T>
{
    string filename;
    fstream file;
public:
    FileService(string filename)
    {
        this->filename = filename;
    }
    FileProcessResult* open(ios_base::openmode mode) override
    {
        FileProcessResult* result = nullptr;
        file.open(filename, mode);
        if (file.is_open())
        {
            result = new FileOpenSuccess();
        }
        else
        {
            result = new FileOpenError();
        }
        return result;
    }
    FileProcessResult* close() override
    {
        file.close();
        return new FileCloseSuccess();
    }
    FileProcessResult* write(T data, ios_base::openmode mode) override
    {
        try
        {
            FileProcessResult* result = this->open(mode);
            result->get_message();
            if (typeid(*result) == typeid(FileOpenSuccess))
            {
                if (mode == ios::app || mode == ios::out || mode == (ios::out | ios::binary) || mode == (ios::app | ios::binary))
                {
                    file.write((char*)&data, sizeof(T));
                }
                else
                {
                    return new FileWriteError();
                }
                result = this->close();
                result->get_message();
                return new FileWriteSuccess();
            }
            else if (typeid(*result) == typeid(FileOpenError))
            {
                return new FileWriteError();
            }
        }
        catch (exception e)
        {
            return new FileWriteError(e);
        }
    }

    FileProcessResult* read(T& data, ios_base::openmode mode) override
    {
        try
        {
            FileProcessResult* result = this->open(mode);
            result->get_message();
            if (typeid(*result) == typeid(FileOpenSuccess))
            {
                if (mode == ios::in || mode == (ios::in | ios::binary))
                {
                    file.read((char*)&data, sizeof(T));
                }
                else
                {
                    return new FileWriteError();
                }
                result = this->close();
                result->get_message();
                return new FileReadSuccess();
            }
            else if (typeid(*result) == typeid(FileOpenError))
            {
                return new FileReadError();
            }
        }
        catch (exception e)
        {
            return new FileReadError(e);
        }
    }
};



template <typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;
        Node(T val) : data(val), next(nullptr) {}
    };
    Node* head;
    Node* tail;

public:
    LinkedList() : head(nullptr), tail(nullptr) {}

    void AddToHead(T value) {
        Node* newNode = new Node(value);
        if (!head) {
            head = tail = newNode;
        }
        else {
            newNode->next = head;
            head = newNode;
        }
    }

    void AddToTail(T value) {
        Node* newNode = new Node(value);
        if (!head) {
            head = tail = newNode;
        }
        else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    LinkedList<T>* Clone() const {
        LinkedList<T>* newList = new LinkedList<T>();

            Node* temp = head;
            while (temp) {
            newList->AddToTail(temp->data);
            temp = temp->next;
        }

        return newList;
    }

    LinkedList<T> operator+(const LinkedList<T>& other) const {
        LinkedList<T> newList;

        Node* temp = head;
        while (temp) {
            newList.AddToTail(temp->data);
            temp = temp->next;
        }

        temp = other.head;
        while (temp) {
            newList.AddToTail(temp->data);
            temp = temp->next;
        }

        return newList;
    }

    LinkedList<T> operator*(const LinkedList<T>& other) const {
        LinkedList<T> newList;

        Node* temp1 = head;
        while (temp1) {
            Node* temp2 = other.head;
            while (temp2) {
                if (temp1->data == temp2->data) {
                    newList.AddToTail(temp1->data);  
                    break;
                }
                temp2 = temp2->next;
            }
            temp1 = temp1->next;
        }

        return newList;
    }

    void Show() const {
        if (!head) {
            cout << "Список порожній" << endl;
            return;
        }

        Node* temp = head;
        while (temp) {
            cout << temp->data << " ";
            temp = temp->next;
        }
        cout << endl;
    }

    void DeleteAll() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
    }

    ~LinkedList() {
        DeleteAll();
    }
};

int main() {
    try
    {
        LinkedList<int> list1;
        list1.AddToHead(10);
        list1.AddToTail(20);
        list1.AddToTail(30);

        FileService<LinkedList<int>> file("data.txt");
        FileProcessResult* result = nullptr;

        result = file.write(list1, ios::out | ios::binary);
        result = file.read(list1 , ios::in | ios::binary);

        result->get_message();

        delete result;
    }
    catch (FileProcessResult* file_error)
    {
        file_error->get_message();
    }
}
     
