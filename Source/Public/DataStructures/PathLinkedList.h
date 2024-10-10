//
// Created by MrHaloka.
//

#ifndef ASTAR_PATHLINKEDLIST_H
#define ASTAR_PATHLINKEDLIST_H

#include "Vector2d.h."

struct PathLinkedList {
protected:
    Vector2d path;
    PathLinkedList* next;
    PathLinkedList* previous;
public:
    PathLinkedList()
    {
        path = Vector2d(0);
        next = nullptr;
        previous = nullptr;
    }

    PathLinkedList(const Vector2d& newPath)
    {
        path = newPath;
        next = nullptr;
        previous = nullptr;
    }

    PathLinkedList* GetNext() const
    {
        return next;
    }

    PathLinkedList* GetPrevious() const
    {
        return previous;
    }

    void SetNext(PathLinkedList* newNext)
    {
        next = newNext;
    }

    void SetPrevious(PathLinkedList* newPrevious)
    {
        previous = newPrevious;
    }

    Vector2d GetPath()
    {
        return path;
    }

    void AddAfter(PathLinkedList* after)
    {
        after->SetNext(this->GetNext());
        if (this->GetNext() != nullptr)
        {
            this->GetNext()->SetPrevious(after);
        }
        this->SetNext(after);
        after->SetPrevious(this);
    }

    void AddBefore(PathLinkedList* before)
    {
        before->SetPrevious(this->GetPrevious());
        if (this->GetPrevious() != nullptr)
        {
            this->GetPrevious()->SetNext(before);
        }
        this->SetPrevious(before);
        before->SetNext(this);
    }

    void SetPath(const Vector2d& newPath)
    {
        path = newPath;
    }

    void Replace(PathLinkedList* replace)
    {
        this->SetNext(replace->GetNext());
        this->SetPrevious(replace->GetPrevious());
        this->SetPath(replace->GetPath());
    }

    void RemoveAndReconnect()
    {
        if (this->GetNext() != nullptr)
        {
            this->GetNext()->SetPrevious(this->GetPrevious());
        }
        if (this->GetPrevious() != nullptr)
        {
            this->GetPrevious()->SetNext(this->GetNext());
        }
    }

    void DeleteList()
    {
        PathLinkedList* current = this->previous;
        while (current != nullptr)
        {
            auto oldPath = current;
            current = current->GetPrevious();
            delete oldPath;
        }
        delete this;
    }
};

#endif //ASTAR_PATHLINKEDLIST_H
