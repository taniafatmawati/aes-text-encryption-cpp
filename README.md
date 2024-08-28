# 🛡️ Cryptography Application: AES Implementation for Text Data Security

---

## 📋 Project Objectives

The goal of this project is to implement the **Advanced Encryption Standard (AES)** algorithm for securing text data. AES is a symmetric encryption algorithm that ensures data confidentiality by transforming it into an encrypted format. This implementation uses AES 128-bit encryption to secure text data.

## 📋 Prerequisites

- **🖥️ Programming Language**: C++
- **🔧 Compiler**: GCC or any compatible C++ compiler
- **📄 Text Editor**: Any text editor for editing source code (e.g., VSCode)

## 🌟 Key Elements

- **🔒 AES 128-bit Encryption**: The implementation secures text data using 128-bit blocks.
- **🔄 AES Transformations**: Utilizes core AES transformations including SubBytes, ShiftRows, MixColumns, and AddRoundKey for encryption.
- **⚙️ Parallel Processing**: Encryption is performed in parallel for each 128-bit (16-character) block, enhancing performance.

## 🚧 Project Constraints

- **Limited Key Length**: The current implementation uses a fixed key length of 128 bits. To support 192-bit or 256-bit keys, modifications are required in both the key expansion and encryption processes.
- **Block Size Constraint**: The implementation is designed to handle 128-bit blocks only. Input data must be padded or truncated to fit this block size.
- **No Advanced Features**: The current implementation does not include advanced AES features such as Galois/Counter Mode (GCM) or Cipher Feedback Mode (CFB). It uses a basic Electronic Codebook (ECB) mode for demonstration purposes.
- **Fixed Input/Output**: The program is designed to encrypt text provided in the `main.cpp` file. For more dynamic input handling, additional modifications are needed.
- **Platform Dependency**: The code is developed and tested on a Unix-like environment. Compatibility with other operating systems may require adjustments.

## 📋 Usage Instructions

1. Clone the Repository:
   ```bash
   git clone https://github.com/taniafatmawati/aes-text-encryption-cpp.git
   cd aes-text-encryption-cpp
   ```

2. Build the Project:
   ```bash
   make
   ```

3. Run the Program:
   ```bash
   ./AES_Project
   ```

4. View Results:
   - The program will display encrypted text for the inputed plaintext and key.

5. Implementation Details:
   - `aes.hpp`: Contains declarations for AES encryption functions.
   - `aes.cpp`: Implements AES transformations and core functionality.
   - `main.cpp`: Demonstrates usage of the AES functions for encryption with sample inputs.

## 📋 Result and Documentation

- **📄 Results**: The program provides encrypted outputs for text data using the AES algorithm.
- **📚 Documentation**: Includes source code files, usage instructions, and additional details in this `README.md`.

## 📝 Conclusion

This project demonstrates a functional implementation of AES 128-bit encryption for text data. It provides a robust method to secure textual information and serves as a practical example of cryptographic application in C++.

Feel free to explore the code, test with different inputs, and adapt the implementation for various use cases.

## 🔗 Additional Notes

- Ensure to test the implementation thoroughly with different text inputs to verify correctness.
- Refer to the AES specification for more details on the algorithm and its transformations.
