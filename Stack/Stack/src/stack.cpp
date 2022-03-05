#include "stack.hpp"

#include <iostream>

int prec(char c) {
     if(c == '^')
        return 3;
    else if(c == '/' || c=='*')
        return 2;
    else if(c == '+' || c == '-')
        return 1;
    else
        return -1;
}

void infixToPostfix(std::string& s) {
 
    Stack<char, 20> st; //For stack operations, we are using C++ built in stack
    std::string result {};
 
    for(int i = 0; i < s.length(); i++) {
        char c {s[i]};
 
        // If the scanned character is
        // an operand, add it to output string.
        if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
            result += c;
 
        // If the scanned character is an
        // ‘(‘, push it to the stack.
        else if(c == '(')
            st.push('(');
 
        // If the scanned character is an ‘)’,
        // pop and to output string from the stack
        // until an ‘(‘ is encountered.
        else if(c == ')') {
            while(st.peek() != '(')
            {
                result += st.pop();
            }
            st.pop();
        }
 
        //If an operator is scanned
        else {
            while(!st.isEmpty() && prec(s[i]) <= prec(st.peek())) {
                result += st.pop();
            }
            st.push(c);
        }
    }
 
    // Pop all the remaining elements from the stack
    while(!st.isEmpty()) {
        result += st.pop();
    }
 
    std::cout << result << '\n';
}

void reverseString(std::string& str) {
    Stack<char, 100> st {};
    for(int i{0}; i < str.length(); ++i) {
        st.push(str[i]);
    }

    for(int i{0}; i < str.length(); ++i) {
        str[i] = st.pop();
    }
}

bool balancedBrackets(std::string& str) {
    Stack<char, 100> stack {};
    
    for(auto element: str) {
        if(element == '(' || element == '[' || element == '{') {
            stack.push(element);
        }
        else if(element == ')' || element == ']' || element == '}') {
            char popped_element { stack.pop() };
            switch(element) {
                case ')':
                    if(popped_element != '(')
                        return false;
                    break;
                case ']':
                    if(popped_element != '[')
                        return false;
                    break;
                case '}':
                    if(popped_element != '{')
                        return false;
                    break;
            }
        }
        
    }
    return true; 

}

// Code to solve stoch span problem

void calculateSpan(int price[], int n, int S[]) {
    Stack<int, 100> stack{};
    stack.push(0);
    S[0] = 1; 

    for(int i {1}; i < n; ++i) {
        while(!stack.isEmpty() && price[stack.peek()] <= price[i]) {
            stack.pop();
        }
        S[i] = (stack.isEmpty()) ? (i + 1) : (i - stack.peek());
        
        stack.push(i);
    }
}
