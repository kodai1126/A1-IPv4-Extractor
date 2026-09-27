#include <iostream>
#include <string>
#include <cctype>

using namespace std;

// Parse digits manually.
// Returns true if a valid number was found under the given rules.
bool parseNumber(const string& str,
                 size_t& pos,
                 int maxDigits,
                 int maxValue,
                 int& value)
{
    size_t start = pos;
    int digitCount = 0;
    value = 0;

    while (pos < str.length() &&
           isdigit(static_cast<unsigned char>(str[pos]))) {

        // Too many digits
        if (digitCount >= maxDigits) {
            return false;
        }

        int digit = str[pos] - '0';
        value = value * 10 + digit;

        digitCount++;
        pos++;
    }

    // Must contain at least one digit
    if (digitCount == 0) {
        return false;
    }

    // Leading zero is only allowed for exactly "0"
    if (digitCount > 1 && str[start] == '0') {
        return false;
    }

    // Must be in allowed range
    if (value > maxValue) {
        return false;
    }

    return true;
}


// Check whether a character would mean that we are
// accidentally taking part of a larger IP-like token.
bool badBoundary(char c)
{
    return isdigit(static_cast<unsigned char>(c)) ||
           c == '.' ||
           c == ':';
}


bool extractIPv4(const string& str,
                 unsigned long& outAddress,
                 int& outPort)
{
    // Required failure values
    outAddress = 0;
    outPort = -1;

    for (size_t start = 0; start < str.length(); start++) {

        // An IPv4 address must start with a digit.
        if (!isdigit(static_cast<unsigned char>(str[start]))) {
            continue;
        }

        // Do not begin in the middle of another IP-like token.
        if (start > 0 && badBoundary(str[start - 1])) {
            continue;
        }

        size_t pos = start;
        int octets[4];
        bool valid = true;

        // Parse four octets
        for (int i = 0; i < 4; i++) {

            if (!parseNumber(str, pos, 3, 255, octets[i])) {
                valid = false;
                break;
            }

            // First three octets must be followed by '.'
            if (i < 3) {
                if (pos >= str.length() || str[pos] != '.') {
                    valid = false;
                    break;
                }

                pos++;  // skip '.'
            }
        }

        if (!valid) {
            continue;
        }

        int port = -1;

        // Optional port
        if (pos < str.length() && str[pos] == ':') {
            pos++;  // skip ':'

            if (!parseNumber(str, pos, 5, 65535, port)) {
                // Colon exists, so an invalid port rejects
                // this IPv4 candidate completely.
                continue;
            }
        }

        // Make sure we did not stop in the middle of
        // a larger IP-like token.
        if (pos < str.length() && badBoundary(str[pos])) {
            continue;
        }

        // Build the 32-bit IPv4 value manually.
        unsigned long address = 0;

        address = address * 256 + octets[0];
        address = address * 256 + octets[1];
        address = address * 256 + octets[2];
        address = address * 256 + octets[3];

        outAddress = address;
        outPort = port;

        return true;
    }

    return false;
}


int main()
{
    string input;

    while (true) {
        cout << "Enter a string (or 'END' to quit): ";
        getline(cin, input);

        if (input == "END") {
            cout << "Program terminated." << endl;
            break;
        }

        unsigned long address;
        int port;

        if (extractIPv4(input, address, port)) {

            unsigned long a = (address >> 24) & 255;
            unsigned long b = (address >> 16) & 255;
            unsigned long c = (address >> 8) & 255;
            unsigned long d = address & 255;

            cout << "Extracted IPv4 address: "
                 << a << "."
                 << b << "."
                 << c << "."
                 << d
                 << " (decimal value: "
                 << address
                 << ", port: ";

            if (port == -1) {
                cout << "none";
            }
            else {
                cout << port;
            }

            cout << ")" << endl;
        }
        else {
            cout << "Invalid input: no valid IPv4 address found" << endl;
        }
    }

    return 0;
}