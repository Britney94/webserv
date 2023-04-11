#!/usr/bin/env python3
import os

# Get the content type of the request
content_type = os.environ.get('CONTENT_TYPE', '')

if content_type == 'multipart/form-data':
    # Get the content length of the request
    content_length = os.environ['CONTENT_LENGTH']
    content_length = int(content_length) if content_length else 0

    # Get the path to the uploaded file
    path_translated = os.environ['PATH_TRANSLATED']

    # Read the raw data from the request
    query_string = os.environ['QUERY_STRING']

    # Go to the good directory
    os.chdir("./")

    # Extract the text and image data from the raw data
    text_data = query_string.split('text=')[1].split('&')[0]
    image_data = query_string.split('filename=')[1]

    # Write the text data to a file
    file_list = path_translated.strip().split('\n')

    # Print a response to the client
    print('<html><head><title>CGI Multipart</title></head><body>')
    print('<h1>CGI Test</h1>')
    print('<table>')
    for file_path in file_list:
        with open(file_path, 'r') as f:
            file_content = f.read()
        print('<p>File path: {}</p>'.format(file_path))
        print('<tr><td><p>{}</p></td></tr>'.format(file_content))
    print('</table>')
    print('</body></html>')

else:
    print('<html><head><title>CGI Multipart</title></head><body>')
    print('<h1>CGI Test</h1>')
    print('<p>Error: Unsupported content type</p>')
    print('</body></html>')