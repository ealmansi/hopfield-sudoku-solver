#ifndef AUXILIARY_MACROS_H
#define AUXILIARY_MACROS_H

#define EXIT_WITH_ERROR_MSG(msg)                        \
      {                                                 \
         cerr << "Error: " << #msg << endl;             \
         exit(-1);                                      \
      }                                                 \

#define PRINT_VAR_NAME(var)								\
      {													\
      	cout << #var << ":" << endl;					\
      }													\

#define PRINT_INT(var)                                  \
      {                                                 \
         PRINT_VAR_NAME(var);							\
         cout << (int)(var) << endl;					\
      }                                                 \

#define PRINT_DOUBLE(var)                               \
      {                                                 \
         PRINT_VAR_NAME(var);							\
         cout << (double)(var) << endl;					\
      }                                                 \

#define PRINT_INT_ARRAY(arr,len)						\
	{													\
		PRINT_VAR_NAME(arr);							\
		cout << "[";									\
		for(int i = 0; i < (len); i++)					\
		{												\
			cout << (int)((arr)[i]);					\
			if( i+1 < (len) ) cout << " ";				\
		}												\
		cout << "]" << endl;							\
	}													\

#define PRINT_INT_MATRIX(mat, rows, cols)				\
	{													\
		PRINT_VAR_NAME(mat);							\
		cout << "[";									\
		for(int i = 0; i < (rows); i++)					\
		{												\
			cout << "[";								\
			for(int j = 0; j < (cols); j++)				\
			{											\
				cout << (int)(mat)[i][j];				\
				if( j+1 < (cols) ) cout << " ";			\
			}											\
			cout << "]";								\
			if( i+1 < (rows) ) cout << "," << endl;		\
		}												\
		cout << "]" << endl;							\
	}													\


#define IN_RANGE(min,val,max)							\
	((min) <= (val) && (val) <= (max))					\

#endif//AUXILIARY_MACROS_H