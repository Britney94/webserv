#!/usr/bin/env python3

import os

# Print the HTTP header
print("<!DOCTYPE html>")
print("<html>")
print("<head>")
print("<title>CGI Environment Variables</title>")
print("</head>")
print("<body>")

# Create an HTML table to display the CGI environment variables
print("<table>")
print("<tr><th>Variable</th><th>Value</th></tr>")
for key, value in os.environ.items():
    print(f"<tr><td>{key}</td><td>{value}</td></tr>")
print("</table>")
print("</body>")
print("</html>")