# AI Usage Disclosure

## 1. Generative AI Tool Used

I used **ChatGPT (GPT-5.6 Sol)** by OpenAI for this assignment.

**Date consulted:** September 27, 2026

I used ChatGPT for the initial C++ code generation, reviewing the generated code, identifying possible issues, suggesting test cases, and correcting the required output formatting.

---

## 2. Initial Prompt

The following is the exact prompt I used to generate the initial version of the program:

> I need help writing a C++ program that extracts one valid IPv4 address from a line of text. The IPv4 address can appear anywhere in the input and may optionally have a port number after it.
>
> Please use this function prototype:
>
> bool extractIPv4(const std::string& str, unsigned long& outAddress, int& outPort);
>
> Requirements:
> - A valid IPv4 address has four octets separated by periods, like A.B.C.D.
> - Each octet must contain 1 to 3 digits and have a value from 0 to 255.
> - Leading zeros are not allowed unless the octet is exactly 0.
> - A port number may appear after the address using a colon, such as 192.168.1.1:8080.
> - The port must contain 1 to 5 digits and have a value from 0 to 65535.
> - Leading zeros are also not allowed in the port unless the port is exactly 0.
> - If a colon is present but the port is invalid, reject the whole IPv4 address.
> - Do not accept partial matches from a longer invalid token.
> - A period or colon directly next to an otherwise valid address should make that candidate invalid.
> - Other unrelated characters in the input should be skipped.
>
> Restrictions:
> - Do not use stoi, atoi, strtol, sscanf, scanf numeric conversions, or similar string-to-number functions.
> - Do not use regular expressions.
> - Do not use inet_aton, inet_pton, inet_addr, or other IP parsing functions.
> - Parse and build all numeric values manually character by character.
>
> On success:
> - Return true.
> - Store the IPv4 address as a 32-bit decimal value in outAddress.
> - Store the port in outPort, or -1 if there is no port.
>
> On failure:
> - Return false.
> - Set outAddress to 0.
> - Set outPort to -1.
>
> Also write a main function that repeatedly asks the user for a line of input until the user enters END. If a valid address is found, print the address, its 32-bit decimal value, and the port or "none". Otherwise, print that no valid IPv4 address was found.
>
> Please write readable C++ code and explain the main parsing logic after the code.

---

## 3. AI-Generated Code

ChatGPT generated the initial implementation of the program.

The AI-generated code included:

- `parseNumber()` to manually convert digit characters into integer values.
- Checks for the maximum number of digits.
- Checks for leading zeros.
- Checks for octet values from 0 to 255.
- Checks for port values from 0 to 65535.
- `badBoundary()` to detect digits, periods, or colons directly adjacent to a candidate.
- `extractIPv4()` to scan the input text and locate a valid IPv4 address.
- Manual construction of the 32-bit IPv4 value.
- A `main()` loop that continued accepting input until `END`.

The program did not use prohibited functions such as `stoi`, `atoi`, regular expressions, or IP-address parsing libraries.

---

## 4. Review of the Initial AI Output

I did not immediately accept the generated code as the final submission.

I compiled and ran the program and compared its behavior with the assignment requirements.

The parsing logic successfully handled the initial valid and invalid cases I tested. However, I found problems in the user-interface portion of the AI-generated program.

The initial code printed:

```text
Enter a line (or END to quit):
```

instead of the required:

```text
Enter a string (or 'END' to quit):
```

The initial success output also used several separate lines:

```text
Valid IPv4 address found.
Address: 192.168.1.1
32-bit decimal value: 3232235777
Port: none
```

The assignment required the following single-line format:

```text
Extracted IPv4 address: 192.168.1.1 (decimal value: 3232235777, port: none)
```

The initial failure message was:

```text
No valid IPv4 address was found.
```

instead of:

```text
Invalid input: no valid IPv4 address found
```

The initial program also exited after receiving `END` without printing:

```text
Program terminated.
```

These were important problems because the assignment explicitly requires the specified output format.

---

## 5. Modifications Made

I modified the `main()` function so that its prompts and output match the assignment requirements.

The changes included:

- Changing the input prompt to:

```text
Enter a string (or 'END' to quit):
```

- Changing valid output to:

```text
Extracted IPv4 address: A.B.C.D (decimal value: N, port: P)
```

- Printing `none` when no port is included.

- Changing invalid output to:

```text
Invalid input: no valid IPv4 address found
```

- Adding:

```text
Program terminated.
```

when the user enters `END`.

I kept the main IPv4 parsing approach from the AI-generated version because testing showed that it correctly handled the tested parsing requirements.

---

## 6. Testing and Verification

After reviewing and modifying the program, I tested both normal inputs and edge cases.

### Basic Valid Cases

```text
192.168.1.1
10.0.0.255:8080
0.0.0.0
255.255.255.255
```

These were correctly accepted.

### Invalid Octet Values

```text
256.1.1.1
255.255.255.256
```

These were correctly rejected because IPv4 octets cannot exceed 255.

### Leading Zero Tests

```text
01.2.3.4
1.02.3.4
1.2.003.4
1.2.3.004
```

These were correctly rejected.

### Port Boundary Tests

```text
1.2.3.4:0
1.2.3.4:1
1.2.3.4:65535
```

These were correctly accepted.

```text
1.2.3.4:65536
1.2.3.4:01
1.2.3.4:00000
```

These were correctly rejected.

### Malformed Port Tests

```text
1.2.3.4:
1.2.3.4::
1.2.3.4:80:
1.2.3.4:80.
1.2.3.4:80:90
```

These were correctly rejected.

### Invalid IPv4 Structure Tests

```text
12.34.56
1.2.3.4.5
1..2.3.4
1.2..3.4
1.2.3..4
.1.2.3.4
1.2.3.4.
```

These were correctly rejected.

### Excessive Digit Tests

```text
1111.2.3.4
1.2222.3.4
1.2.3333.4
1.2.3.4444
```

These were correctly rejected.

### Noisy Text Tests

```text
abc1.2.3.4xyz
```

was correctly interpreted as containing the valid address:

```text
1.2.3.4
```

because unrelated characters are treated as garbage.

Similarly:

```text
192a168.1.1.1
```

correctly produced:

```text
168.1.1.1
```

which matches the expected behavior of skipping unrelated characters.

### Partial-Match and Boundary Tests

I also tested cases specifically intended to make sure the program did not extract a valid-looking substring from a larger malformed token:

```text
999.1.2.3.4
1.2.3.4.5
1234.1.2.3
1.2.3.4444
1.2.3.4:123456
1.2.3.4:65535.
1.2.3.4:65535:
1.2.3.4::80
```

All of these were correctly rejected.

Finally, I tested the maximum valid IPv4 address and port together:

```text
255.255.255.255:65535
```

The program correctly returned:

```text
Extracted IPv4 address: 255.255.255.255 (decimal value: 4294967295, port: 65535)
```

---

## 7. What I Learned From Reviewing the AI Output

The main lesson from this assignment was that code generated by an AI tool should not be assumed to be correct just because it compiles and appears reasonable.

In this case, the initial parsing algorithm performed well on the test cases I tried, but the AI did not exactly follow several user-interface requirements from the assignment.

The missing termination message and incorrect output format were easy to overlook because they did not affect the parsing algorithm itself.

Testing the program against both the assignment specification and unusual edge cases helped me verify the behavior instead of accepting the generated code without checking it.

I also gained a better understanding of how the parser works. In particular, I reviewed how numeric values are manually accumulated using logic equivalent to:

```cpp
value = value * 10 + digit;
```

and how the IPv4 decimal representation is constructed one octet at a time using multiplication by 256.

---

## 8. Code Attribution

The initial implementation of the parsing functions and program structure was generated with assistance from ChatGPT.

I reviewed, tested, and modified the generated program.

My modifications focused mainly on:

- Matching the exact required input prompt.
- Matching the exact required valid-output format.
- Matching the required invalid-output message.
- Adding the required program termination message.
- Designing and running additional edge-case tests.
- Verifying valid and invalid IPv4 and port boundaries.

ChatGPT was also used to suggest additional edge cases that I then manually ran against the program.

---

## 9. Verification Statement

I reviewed the final submitted code and understand how its parsing logic works, including manual digit accumulation, octet validation, port validation, candidate boundary checking, and construction of the 32-bit IPv4 value.

I tested the program using valid inputs, invalid inputs, boundary values, malformed addresses, malformed ports, leading-zero cases, surrounding garbage text, and partial-match cases.

Based on the tests I performed, the final version works as intended for the assignment requirements.

At the time of submission, I am not aware of any unresolved bugs or unexpected behavior in the program.