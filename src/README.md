# Error Context Library

A C library for capturing detailed error context in single-threaded applications. Provides structured error information including file, line number, function name, and custom descriptions for better debugging and logging.

## Features

- **Full Error Context**: Captures file, line number, function name, error code, and description
- **Memory Safe**: Buffer-safe string handling with proper bounds checking
- **Flexible Usage**: Supports both stack and heap allocation
- **Simple Integration**: No changes to existing function signatures required
- **Single-Threaded Focus**: Designed explicitly for single-threaded environments

## Quick Start

```c
#include "errorhandling.h"

// Initialize the global context once at application start
if (Error_InitGlobalCtx() != ERROR_SUCCESS) {
    fprintf(stderr, "Failed to initialize error system\n");
    return 1;
}

// Enable logging to a file
if (Error_SetLogFile("app.log") != ERROR_SUCCESS) {
    fprintf(stderr, "Failed to set log file\n");
    return 1;
}

// Enable logging
Error_SetLogON();

// Example usage in a function
int process_data(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        ErrorStruct_t error;
        Error_InitErrorStruct(&error, 
                             ERR_FILE_OPEN, 
                             __LINE__, 
                             __func__, 
                             __FILE__,
                             "Failed to open file: %s", 
                             filename);
        Error_LogError(&error);
        return ERR_FILE_OPEN;
    }
    // ... process file
    fclose(file);
    return SUCCESS;
}

// Cleanup at application exit
Error_CleanupGlobalCtx();
```

## API Reference

### Core Structures

```c
// Error configuration (global, single instance)
typedef struct ErrorConfig_s ErrorConfig_t;

// Error context structure
typedef struct ErrorStruct_s {
    int error_code;                      // Numeric error code
    size_t line_number;                  // Line where error occurred
    char function_name[FUNCTION_NAME_MAX];  // Function name
    char file_name[FILE_NAME_MAX];       // Source file name  
    char error_description[ERROR_DESCRIPTION_MAX];  // Error description
} ErrorStruct_t;
```

### Configuration Functions

#### `Error_InitGlobalCtx`
Initializes the global error context.

**Returns:**
- `ERROR_SUCCESS` (0) - Context initialized successfully
- `ERROR_GLOBALCTX_DOUBLEINIT` (-204) - Context already initialized
- `ERROR_MEMORY_ALLOCATION` (-2) - Memory allocation failed

**Usage:**
```c
// Call once at application start
int result = Error_InitGlobalCtx();
if (result != ERROR_SUCCESS) {
    // Handle initialization failure
}
```

#### `Error_CleanupGlobalCtx`
Cleans up the global error context and releases resources.

**Returns:**
- `ERROR_SUCCESS` (0) - Cleanup successful
- `ERROR_NULL_VALUE_GIVEN` (-6) - Global context was NULL

**Usage:**
```c
// Call once at application exit
Error_CleanupGlobalCtx();
```

#### `Error_SetLogFile`
Sets the log file path. If logging is enabled, errors will be written to this file.

**Parameters:**
- `log_file_path`: Path to log file (opens in append mode)

**Returns:**
- `ERROR_SUCCESS` (0) - Log file set successfully
- `ERROR_GLOBALCTX_NULL` (-200) - Global context not initialized
- `ERROR_NULL_VALUE_GIVEN` (-6) - NULL file path provided
- System error if file cannot be opened

**Usage:**
```c
// Log to a file
Error_SetLogFile("errors.log");

// Log to stderr (pass NULL)
Error_SetLogFile(NULL);
```

#### `Error_SetLogON` / `Error_SetLogOff`
Enables or disables error logging.

**Returns:**
- `ERROR_SUCCESS` (0) - Operation successful
- `ERROR_GLOBALCTX_NULL` (-200) - Global context not initialized

**Usage:**
```c
// Enable logging
Error_SetLogON();

// Disable logging (errors will be silently ignored)
Error_SetLogOff();
```

### Error Structure Management

#### `Error_InitErrorStruct`
Initializes an error structure with context information.

**Parameters:**
- `errstct`: Pointer to store the initialized error structure
- `errorcode`: Numeric error code
- `line_number`: Line number where error occurred (use `__LINE__`)
- `function_name`: Function name (use `__func__`)
- `file_name`: Source file name (use `__FILE__`)
- `error_description`: Format string for error description (followed by optional arguments)

**Returns:**
- `ERROR_SUCCESS` (0) - Initialization successful
- `ERROR_NULL_VALUE_GIVEN` (-6) - NULL parameter provided
- `ERROR_MEMORY_ALLOCATION` (-2) - Memory allocation failed
- `ERROR_ERRORSTRUCT_INIT` (-203) - String truncation occurred

**Usage:**
```c
// Stack allocation (recommended for most cases)
ErrorStruct_t error;
int result = Error_InitErrorStruct(&error, 
                                   ERR_INVALID_INPUT,
                                   __LINE__,
                                   __func__,
                                   __FILE__,
                                   "Invalid input value: %d", 
                                   value);

// Heap allocation (for returning errors)
ErrorStruct_t *error_ptr = NULL;
result = Error_InitErrorStruct(&error_ptr,
                               ERR_MEMORY,
                               __LINE__,
                               __func__,
                               __FILE__,
                               "Out of memory");
```

#### `Error_DestroyErrorStruct`
Frees resources associated with an error structure.

**Parameters:**
- `errstct`: Error structure to destroy (can be NULL)

**Returns:**
- `ERROR_SUCCESS` (0) - Destruction successful
- `ERROR_NULL_VALUE_GIVEN` (-6) - NULL parameter provided

**Usage:**
```c
// For stack-allocated structures (no cleanup needed)
ErrorStruct_t error;
// ... use error
// No need to call DestroyErrorStruct

// For heap-allocated structures
ErrorStruct_t *error_ptr = malloc(sizeof(ErrorStruct_t));
// ... use error_ptr
Error_DestroyErrorStruct(error_ptr);
```

### Logging Function

#### `Error_LogError`
Logs an error structure to the configured output.

**Parameters:**
- `error_stct`: Error structure to log (cannot be NULL if logging is enabled)

**Returns:**
- `ERROR_SUCCESS` (0) - Logging successful or logging disabled
- `ERROR_NULL_VALUE_GIVEN` (-6) - Log file is NULL
- `ERROR_LOGERROR` (-202) - fprintf failed

**Usage:**
```c
ErrorStruct_t error;
Error_InitErrorStruct(&error, ...);

// Log the error (if logging is enabled)
Error_LogError(&error);
```

## Error Codes

| Code | Value | Description |
|------|-------|-------------|
| `ERROR_SUCCESS` | 0 | Operation completed successfully |
| `ERROR_MEMORY_ALLOCATION` | -2 | Memory allocation failed |
| `ERROR_NULL_VALUE_GIVEN` | -6 | NULL parameter provided where not allowed |
| `ERROR_GLOBALCTX_NULL` | -200 | Global context not initialized |
| `ERROR_INVALID_ERROR` | -201 | Invalid error structure |
| `ERROR_LOGERROR` | -202 | Failed to write log entry |
| `ERROR_ERRORSTRUCT_INIT` | -203 | Error structure initialization failed |
| `ERROR_GLOBALCTX_DOUBLEINIT` | -204 | Attempt to initialize global context twice |

## Configuration Constants

```c
#define FUNCTION_NAME_MAX 256        // Maximum function name length
#define FILE_NAME_MAX 4098           // Maximum file path length  
#define ERROR_DESCRIPTION_MAX 4098   // Maximum error description length
#define LOG_ON 1                     // Logging enabled
#define LOG_OFF 0                    // Logging disabled
```

## Usage Patterns

### Pattern 1: Immediate Logging (Most Common)
```c
int validate_input(int value) {
    if (value < 0 || value > 100) {
        ErrorStruct_t error;
        Error_InitErrorStruct(&error,
                             ERR_INVALID_RANGE,
                             __LINE__,
                             __func__,
                             __FILE__,
                             "Value %d outside valid range (0-100)",
                             value);
        Error_LogError(&error);
        return ERR_INVALID_RANGE;
    }
    return SUCCESS;
}
```

### Pattern 2: Error Return with Context
```c
ErrorStruct_t* read_configuration(const char *path) {
    FILE *file = fopen(path, "r");
    if (!file) {
        ErrorStruct_t *error = NULL;
        Error_InitErrorStruct(&error,
                             ERR_CONFIG_READ,
                             __LINE__,
                             __func__,
                             __FILE__,
                             "Cannot open configuration file: %s",
                             path);
        return error;  // Caller must free this
    }
    // ... process file
    fclose(file);
    return NULL;  // No error
}
```

### Pattern 3: Debug Tracing
```c
#ifdef DEBUG
    ErrorStruct_t trace;
    Error_InitErrorStruct(&trace,
                         DEBUG_TRACE,
                         __LINE__,
                         __func__,
                         __FILE__,
                         "Entering function with param: %s",
                         param);
    Error_LogError(&trace);
#endif
```

## Best Practices

### 1. Initialize Once
```c
// At application start
int main(void) {
    if (Error_InitGlobalCtx() != ERROR_SUCCESS) {
        // Handle failure - cannot use error library itself
        fprintf(stderr, "Critical: Cannot initialize error system\n");
        return 1;
    }
    
    // Configure logging
    Error_SetLogFile("application.log");
    Error_SetLogON();
    
    // ... application logic
    
    Error_CleanupGlobalCtx();
    return 0;
}
```

### 2. Use Stack Allocation When Possible
```c
// Prefer this (no manual cleanup):
ErrorStruct_t error;
Error_InitErrorStruct(&error, ...);
Error_LogError(&error);

// Over this (requires manual cleanup):
ErrorStruct_t *error = malloc(sizeof(ErrorStruct_t));
Error_InitErrorStruct(&error, ...);
Error_LogError(error);
Error_DestroyErrorStruct(error);
```

### 3. Check Return Values
```c
ErrorStruct_t error;
int result = Error_InitErrorStruct(&error, ...);
if (result != ERROR_SUCCESS) {
    // String truncation or allocation failure
    fprintf(stderr, "Failed to create error context: %d\n", result);
    // Handle appropriately
}
```

### 4. Consistent Error Code Ranges
Define application-specific error codes in separate ranges:

```c
// System errors: -1 to -99
#define ERR_MEMORY      -1
#define ERR_FILE_OPEN   -2

// Application errors: -100 to -199  
#define ERR_INVALID_INPUT -100
#define ERR_CONFIG_PARSE  -101

// Business logic errors: -200 to -299
#define ERR_USER_NOT_FOUND -200
#define ERR_INSUFFICIENT_FUNDS -201
```

## Integration with Existing Code

### Wrapping Legacy Functions
```c
// Legacy function returns simple error code
int legacy_operation(int param) {
    if (param < 0) return -1;
    return 0;
}

// Wrapped version with context
int wrapped_operation(int param) {
    int result = legacy_operation(param);
    if (result < 0) {
        ErrorStruct_t error;
        Error_InitErrorStruct(&error,
                             ERR_LEGACY_FAILURE,
                             __LINE__,
                             __func__,
                             __FILE__,
                             "Legacy operation failed with param=%d, code=%d",
                             param, result);
        Error_LogError(&error);
    }
    return result;
}
```

### Error Context Macros (Optional)
```c
// Convenience macro for common pattern
#define LOG_ERROR(code, ...) \
    do { \
        ErrorStruct_t __err; \
        if (Error_InitErrorStruct(&__err, (code), __LINE__, __func__, __FILE__, __VA_ARGS__) == ERROR_SUCCESS) { \
            Error_LogError(&__err); \
        } \
    } while(0)

// Usage
LOG_ERROR(ERR_FILE_MISSING, "Required file not found: %s", filename);
```

## Limitations

1. **Single-Threaded Only**: Not designed for concurrent access
2. **Global Configuration**: One logging configuration for entire application
3. **Manual Context Capture**: Requires explicit error structure creation
4. **No Error Chaining**: Each error is independent (call stack provides implicit chain)

## Dependencies

- Standard C library (stdlib.h, stdio.h, string.h)
- No external dependencies

## License

Use freely in personal and commercial projects. Modify as needed. Attribution appreciated but not required.
