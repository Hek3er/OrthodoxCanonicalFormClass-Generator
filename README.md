# Orthodox Canonical Form Class Generator

## Overview

This project provides a command-line tool to automatically generate C++ classes following the Orthodox Canonical Form. It simplifies the process of creating well-structured C++ classes by generating both header (.hpp) and implementation (.cpp) files. Additionally, it can create a Makefile and a main.cpp file if they don't already exist in the project directory.

## Features

- Generate Orthodox Canonical Form C++ classes
- Create both .hpp and .cpp files for each class
- Optional template-based generation
- Automatic creation of Makefile and main.cpp (if not present)
- Support for multiple class generation in a single command

## Installation

1. Clone this repository:
   ```
   git clone https://github.com/yourusername/orthodox-class-generator.git
   cd orthodox-class-generator
   ```

2. Build the project:
   ```
   make
   ```

3. Add the following line to your `.zshrc` file to make the command available globally:
   ```
   export PATH=$PATH:/path/to/orthodox-class-generator  // replace with the repository path
   ```

4. Reload your shell configuration:
   ```
   source ~/.zshrc
   ```

## Usage

### Basic Usage

To generate one or more classes:

```
orth [classname1] [classname2] ...
```

### Template-based Generation

To use custom templates for generation:

```
orth -t [classname1] [classname2] ...
```

This will use the templates provided in the `templates` folder.

## Project Structure

```
.
├── main.cpp
├── orth
├── templates
│   ├── samplecpp
│   └── samplehpp
└── README.md
```

- `main.cpp`: Main program file
- `orth`: The executable command-line tool
- `templates/`: Folder containing custom templates
  - `samplecpp`: Template for .cpp files
  - `samplehpp`: Template for .hpp files

## Custom Templates

You can customize the generated code by modifying the template files in the `templates` folder. Use `$REP` as a placeholder for the class name in your templates.

## Orthodox Canonical Form

The Orthodox Canonical Form is a standard way of designing C++ classes that ensures they behave correctly when used with value semantics. A class in Orthodox Canonical Form includes:

1. Default constructor
2. Copy constructor
3. Copy assignment operator
4. Destructor

These four special member functions, when properly implemented, guarantee that objects of the class can be created, copied, assigned, and destroyed without issues.

This tool simplifies the process of creating classes that adhere to this form, promoting better C++ programming practices.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.
