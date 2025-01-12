#ifndef NOP_VECTOR_MACRO_DEFINITIONS_HPP /* Begin macro definitions header file */
#define NOP_VECTOR_MACRO_DEFINITIONS_HPP 1

#ifdef __NOP_STL_LIKE__
  /* Macro interface */
  #define __NOP_VECTOR_MAX_SIZE__ max_size
  #define __NOP_VECTOR_GET_ALLOCATOR__ get_allocator
  #define __NOP_VECTOR_SHRINK_TO_FIT__ shrink_to_fit
  #define __NOP_VECTOR_PUSH_BACK__ push_back
  #define __NOP_VECTOR_POP_BACK__ pop_back
  #define __NOP_VECTOR_TO_STRING__ to_string

  #define __NOP_VECTOR_STR_SHRINK_TO_FIT__ "shrink_to_fit"
  #define __NOP_VECTOR_STR_PUSH_BACK__ "push_back"
  #define __NOP_VECTOR_STR_POP_BACK__ "pop_back"
  #define __NOP_VECTOR_STR_TO_STRING__ "to_string"

  /* Macro types */
  #define __NOP_VECTOR__ vector
  #define __NOP_VECTOR_ITERATOR__ iterator
  #define __NOP_VECTOR_PROXY_ITERATOR__ proxy_iterator
  #define __NOP_VECTOR_CONST_ITERATOR__ const_iterator
  #define __NOP_VECTOR_SIZE_TYPE__ size_type
  #define __NOP_VECTOR_VALUE_TYPE__ value_type
  #define __NOP_VECTOR_ALLOCATOR_TYPE__ allocator_type
  #define __NOP_VECTOR_DIFFERENCE_TYPE__ difference_type
  #define __NOP_VECTOR_CONST_REFERENCE__ const_reference
  #define __NOP_VECTOR_CONST_POINTER__ const_pointer

  #define __NOP_VECTOR_STR__ "vector"
  #define __NOP_VECTOR_STR_SIZE_TYPE__ "size_type"
#else
  /* Macro interface */
  #define __NOP_VECTOR_MAX_SIZE__ maxSize
  #define __NOP_VECTOR_GET_ALLOCATOR__ getAllocator
  #define __NOP_VECTOR_SHRINK_TO_FIT__ shrinkToFit
  #define __NOP_VECTOR_PUSH_BACK__ pushBack
  #define __NOP_VECTOR_POP_BACK__ popBack
  #define __NOP_VECTOR_TO_STRING__ toString

  #define __NOP_VECTOR_STR_SHRINK_TO_FIT__ "shrinkToFit"
  #define __NOP_VECTOR_STR_PUSH_BACK__ "pushBack"
  #define __NOP_VECTOR_STR_POP_BACK__ "popBack"
  #define __NOP_VECTOR_STR_TO_STRING__ "toString"

  /* Macro types */
  #define __NOP_VECTOR__ Vector
  #define __NOP_VECTOR_ITERATOR__ Iterator
  #define __NOP_VECTOR_PROXY_ITERATOR__ ProxyIterator
  #define __NOP_VECTOR_CONST_ITERATOR__ ConstIterator
  #define __NOP_VECTOR_SIZE_TYPE__ sizeType
  #define __NOP_VECTOR_VALUE_TYPE__ valueType
  #define __NOP_VECTOR_ALLOCATOR_TYPE__ allocatorType
  #define __NOP_VECTOR_DIFFERENCE_TYPE__ differenceType
  #define __NOP_VECTOR_CONST_REFERENCE__ constReference
  #define __NOP_VECTOR_CONST_POINTER__ constPointer

  #define __NOP_VECTOR_STR__ "Vector"
  #define __NOP_VECTOR_STR_SIZE_TYPE__ "sizeType"
#endif

#define __NOP_VECTOR_POINTER__ pointer
#define __NOP_VECTOR_REFERENCE__ reference

#endif /* End macro definitions header file */
