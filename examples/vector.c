#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    void *data;
    size_t size;
    size_t capacity;
    size_t element_size;
} Vector;

Vector *vector_init(size_t element_size, size_t initial_capacity)
{
    printf("Allocating memory.\n");
    Vector *vec = (Vector *)malloc(sizeof(Vector));
    if (!vec)
    {
        printf("Error: Unable to allocate memory for vector struct\n");
        return NULL;
    }
    vec->size = 0;
    vec->capacity = initial_capacity;
    vec->element_size = element_size;
    vec->data = malloc(vec->capacity * vec->element_size);
    if (!vec->data)
    {
        printf("Error: Unable to allocate memory for vector data\n");
        free(vec);
        return NULL;
    }
    return vec;
}

void vector_free(Vector *vec)
{
    printf("Deallocating memory.\n");
    free(vec->data);
    free(vec);
}

void vector_push_back(Vector *vec, const void *value)
{
    if (vec->size >= vec->capacity)
    {
        printf("Reallocating memory.\n");
        vec->capacity *= 2;
        void *new_data = realloc(vec->data, vec->capacity * vec->element_size);
        if (new_data == NULL)
        {
            printf("Error: Unable to allocate memory for vector expansion\n");
            vector_free(vec);
            exit(EXIT_FAILURE);
        }
        vec->data = new_data;
    }
    memcpy((char *)vec->data + vec->size * vec->element_size, value, vec->element_size);
    vec->size += 1;
}

void *vector_at(Vector *vec, size_t index)
{
    if (index >= vec->size)
    {
        printf("Index out of bounds: %zu\n", index);
        return NULL;
    }
    return (char *)vec->data + index * vec->element_size;
}

typedef struct
{
    Vector *vec;
    size_t index;
} VectorIterator;

VectorIterator vector_begin(Vector *vec)
{
    VectorIterator it;
    it.vec = vec;
    it.index = 0;
    return it;
}

VectorIterator vector_end(Vector *vec)
{
    VectorIterator it;
    it.vec = vec;
    it.index = vec->size;
    return it;
}

int vector_iterator_has_next(VectorIterator *it)
{
    return (it->index < it->vec->size);
}

int *vector_iterator_next(VectorIterator *it)
{
    if (!vector_iterator_has_next(it))
    {
        printf("Iterator reached the end.\n");
        return NULL;
    }
    void *value_ptr = (char *)it->vec->data + it->index * it->vec->element_size;
    it->index += 1;
    return value_ptr;
}

int main(void)
{
    size_t capacity = 10;
    Vector *vec = vector_init(sizeof(double), capacity);
    if (!vec)
    {
        printf("Error: Unable to initialize vector\n");
        return EXIT_FAILURE;
    }

    // Add values
    for (int i = 0; i < 30; ++i)
    {
        double value = (double)i * 5.0;
        vector_push_back(vec, &value);
    }
    printf("\n");

    // Print values
    for (size_t i = 0; i < vec->size; ++i)
    {
        void *value_ptr = vector_at(vec, i);
        if (value_ptr == NULL)
        {
            break;
        }
        printf("Element %ld = %lf \n", i, *((double *)value_ptr));
    }
    printf("\n");

    // Iterator
    VectorIterator it = vector_begin(vec);
    while (vector_iterator_has_next(&it))
    {
        double *value_ptr = (double *)vector_iterator_next(&it);
        if (value_ptr)
        {
            printf("Element %ld = %lf \n", it.index, *value_ptr);
        }
    }
    printf("\n");

    // Try to access index out of bounds
    double *value_ptr = (double *)vector_at(vec, 100000);

    vector_free(vec);

    return EXIT_SUCCESS;
}