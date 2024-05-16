# Grexp - Advanced Text Processing Tool

Grexp is a sophisticated text processing tool inspired by the capabilities of `grep`. It extends the basic functionalities with a comprehensive set of regex features, allowing for complex pattern matching and manipulation in text files and streams.

## Features

- **Regular Expression Support**: Full support for regular expressions including backreferences, group captures, and more.
- **Character Classes**: Use standard and custom character classes to match specific types of characters.
- **Quantifiers**: Includes support for greedy and non-greedy quantifiers such as `+` (one or more), `*` (zero or more), and `?` (zero or one).
- **Anchors and Boundaries**: Match patterns at the start (`^`), end (`$`), or at word boundaries (`\b`).
- **Alternation**: Support for alternation (`|`) allows matching one of many possible patterns.
- **Advanced Searching**: Perform searches across multiple files or input streams, with detailed control over matching behavior.

## Installation

To install Grexp, follow these steps to clone the repository and compile the source code using a C++ compiler supporting C++11 or later.

```bash
# Clone the repository
git clone https://github.com/yourusername/grexp.git

# Change to the directory
cd grexp

# Compile
g++ -o grexp src/*.cpp -std=c++11
```

Ensure that the script is executable:

```bash
chmod +x grexp
```

## Usage

Grexp can process text from files or standard input. Here's how to use it:

```bash
# Using standard input
echo "your text here" | ./grexp "your_regex_here"

# Processing a file
./grexp "your_regex_here" file.txt
```

### Command-Line Options

Grexp supports several options to modify its behavior:

- `-E` : Interpret the pattern as an extended regular expression (implicitly enabled).
- `-i` : Ignore case distinctions in both the pattern and the input files.
- `-v` : Invert the sense of matching, to select non-matching lines.

## Example Usage

To demonstrate Grexp's capabilities, consider the following command:

```bash
echo "Grexp is great" | ./grexp "^G.+t$"
```

This searches for lines that start with "G" and end with "t", covering the entire line.

## Demo

Watch Grexp in action in this brief demo, which illustrates some of its powerful features and how to use them effectively.

[Watch the Demo](https://www.loom.com/share/6fcfe1a2adc54a8dad7a8e77cee22f29?sid=86517cf7-8c2a-4d9d-9acc-53bc5f0fea34)

## Contributing

Contributions are welcome! Please fork the repository, make your changes, and submit a pull request.

## License

Grexp is open-sourced under the MIT License. See the `LICENSE` file for more details.


