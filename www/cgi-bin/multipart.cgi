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

    # Extract the text and image data from the raw data
    text_data = query_string.split('text=')[1].split('&')[0]
    image_data = query_string.split('filename=')[1]

    # Write the text data to a file
    file_list = path_translated.strip().split('\n')

    os.chdir('./')

    # Check if the path translated is empty
    if path_translated == '':
        print('<html><head><title>CGI Multipart</title></head><body>')
        print('<h1>CGI Multipart</h1>')
        print('<p>Error: No file uploaded</p>')
        print('</body></html>')
        exit()

    # Print a response to the client
    print('<html><head><title>CGI Multipart</title></head><body>')
    print('<h1>CGI Multipart</h1>')
    print('<p>Path translated: {}</p>'.format(path_translated))
    print('<table>')
    for file_path in file_list:
        if '.svg' in file_path:
            with open(file_path, 'r') as f:
                file_content = f.read()
            print(file_content)
        elif '.base64' in file_path:
            with open(file_path, 'r') as f:
                file_content = f.read()
            source = "data:image/png;base64," + file_content
            print('<tr><td><img src="{}" alt="Image"></td></tr>'.format(source))
        else:
            filename = file_path.split("/")[-1]
            source = "../uploads/" + filename;
            print('<img src="',source,'" alt="Image">')
    print('</table>')
    print('</body></html>')

else:
    print('<html><head><title>CGI Multipart</title></head><body>')
    print('<h1>CGI Multipart</h1>')
    print('<p>Error: Unsupported content type</p>')
    print('</body></html>')
