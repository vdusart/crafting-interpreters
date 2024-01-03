#include <stdio.h>
#include <stdlib.h>

typedef struct DoublyLinkedNode {
    struct DoublyLinkedNode* prev;
    struct DoublyLinkedNode* next;
    int value;
} DoublyLinkedNode;

void display_line() {
    int N = 20;
    while (N--) {
        printf("-");
    }
    printf("\n");
}

void display_list(DoublyLinkedNode* start) {
    DoublyLinkedNode *tmp = start;

    printf("NULL");
    while (tmp != NULL) {
        printf(" <-> %d", tmp->value);
        tmp = tmp->next;
    }
    printf(" <-> NULL\n");
    display_line();
}

// Insert
DoublyLinkedNode* insert(DoublyLinkedNode* head, int index, int value) {
    if (index < 0) {
        printf("Index MUST be > 0\n\t(index: %d)\n", index);
        return head;
    }

    if (index == 0) {
        DoublyLinkedNode *new_node = malloc(sizeof(DoublyLinkedNode) * 1);

        new_node->prev = NULL;
        new_node->next = head;
        new_node->value = value;
        return new_node;
    }

    int i = 0;
    DoublyLinkedNode *current_node = head;
    while (current_node != NULL && i < index - 1) {
        current_node = current_node->next;
        i++;
    }

    if (current_node == NULL) {
        printf("Impossible to add at this position, list contains %d element(s)\n", i);
        return NULL;
    }
    DoublyLinkedNode *new_node = malloc(sizeof(DoublyLinkedNode) * 1);

    new_node->prev = current_node;
    new_node->next = current_node->next;
    new_node->value = value;

    current_node->next = new_node;
    return head;
}

// Get index of a value
int find_value(DoublyLinkedNode* head, int value) {
    int index = 0;

    while (head != NULL && head->value != value) {
        index++;
        head = head->next;
    }

    return (head != NULL) ? index : -1;
}

// Get value at some index
int find_index(DoublyLinkedNode* head, int index) {
    int i = 0;
    while (head != NULL && i != index) {
        head = head->next;
        i++;
    }

    if (head == NULL) {
        printf("List only contains %d element(s) [not %d]\n", i, index);
        return -1;
    }

    return head->value;
}

// Delete element from DLL
DoublyLinkedNode *delete(DoublyLinkedNode* head, int index) {
    if (index < 0) {
        printf("Impossible to delete item at negative pos\n");
        return head;
    }

    if (head == NULL) {
        printf("The list is already empty\n");
        return NULL;
    }
    if (index == 0) {
        DoublyLinkedNode *new_head = head->next;
        free(head);
        return new_head;
    }

    DoublyLinkedNode *prev_node = NULL;
    DoublyLinkedNode *current_node = head;
    int i = 0;
    while (current_node != NULL && i < index) {
        i++;
        prev_node = current_node;
        current_node = current_node->next;
    }

    if (prev_node == NULL) {
        return head;
    }

    if (current_node == NULL) {
        printf("Impossible to add at this position, list contains %d element(s)\n", i);
        return head;
    }

    prev_node->next = current_node->next;

    return head;
}

void free_doubly_linked_list(DoublyLinkedNode* head) {
    while (head != NULL) {
        DoublyLinkedNode *next = head->next;
        free(head);
        head = next;
    }
}


int main() {
    DoublyLinkedNode* list = NULL;

    printf("Empty Doubly Linked List\n");
    display_list(list);

    printf("Try to add a value in a negative position\n");
    list = insert(list, -1, 42);
    display_list(list);

    printf("Insert 42 in position 1\n");
    list = insert(list, 1, 42);
    display_list(list);

    printf("Insert 42 in position 0\n");
    list = insert(list, 0, 42);
    display_list(list);

    printf("Insert 44 in position 1\n");
    list = insert(list, 1, 44);
    display_list(list);

    printf("Insert 43 in position 1\n");
    list = insert(list, 1, 43);
    display_list(list);

    printf("Insert 45 in position 3\n");
    list = insert(list, 3, 45);
    display_list(list);

    printf("Find value 43 in list at pos %d\n", find_value(list, 43));
    display_list(list);

    printf("Find value 55 in list at pos %d\n", find_value(list, 55));
    display_list(list);

    printf("Get value %d at pos 0\n", find_index(list, 0));
    display_list(list);

    printf("Get value %d at pos 3\n", find_index(list, 3));
    display_list(list);

    printf("Get value %d at pos 10\n", find_index(list, 10));
    display_list(list);

    printf("Delete item at pos 2\n");
    list = delete(list, 2);
    display_list(list);

    printf("Delete item at pos 0\n");
    list = delete(list, 0);
    display_list(list);

    printf("Delete item at pos 10\n");
    list = delete(list, 10);
    display_list(list);

    printf("Delete item at pos 0\n");
    list = delete(list, 0);
    display_list(list);

    printf("Delete item at pos 0\n");
    list = delete(list, 0);
    display_list(list);

    printf("Delete item at pos 0\n");
    list = delete(list, 0);
    display_list(list);

    free_doubly_linked_list(list);

    return 0;
}
