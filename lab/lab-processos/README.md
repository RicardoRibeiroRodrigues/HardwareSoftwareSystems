
Como foi utilizado uma divisão em pastas e arquivos, o comando para compilar ficou: 

``bash
gcc -Wall -Wno-unused-result -Og -g web_downloader.c utils/utils.c utils/str_din.c -o web_downloader -l curl
```