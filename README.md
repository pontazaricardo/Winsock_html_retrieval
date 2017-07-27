# Winsock_html_retrieval

This is a c++ project that connects to a website (input by the user) via sockets, downloads the site's html and looks for any links in the site (<a href = ''> tags).

## Usage

Just import the project to Visual Studio (or can use different IDE), keeping in mind the needed header modifications.

### Visual Studio

Just import the project and rebuild.

### Dev c++

1. Open the *main.cpp* file.
2. Go to *Tools - compiler options*.
    1. Check the *Add these commands to the linker command line*.
    2. Add 
	```
	-lws2_32
	```	
	in the box, and then click on OK.
	
## Results

You will get in console the links, and also the program will generate a *obtainedHTML.txt* file with both the html and the links.

## Notes

1. If the links are of a different form (not <a href =''> tags), the program will not be able to find them.
2. If a site has temporal or permanent forwarding, it will not work. Please use the already forwarded address instead.
3. Do not use *http://* neither *https://*. A normal *www.foo.bar* will work.

## Examples

We tested it with different sites:
1. www.ntu.edu.tw
2. www.nthu.edu.tw
3. arcadianhome.com
without any problems.

![Run example](/images/run.png?raw=true)