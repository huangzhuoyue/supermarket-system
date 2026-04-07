import os
import re

# 1. Fix missing includes
files_to_patch = [
    'src/core/auth.c', 'src/core/member.c', 'src/core/product.c', 
    'src/core/sales.c', 'src/cli/sales_ui.c', 'src/cli/main.c'
]
for file_path in files_to_patch:
    if os.path.exists(file_path):
        content = open(file_path, 'r', encoding='utf-8').read()
        if 'file_utils.h' not in content:
            content = re.sub(r'(#include.*?\n)', r'\1#include "file_utils.h"\n', content, count=1)
            open(file_path, 'w', encoding='utf-8').write(content)

# 2. Fix api_member.c missing deleteMember
api_member_c = open('src/api/api_member.c', 'r', encoding='utf-8').read()
api_member_c = re.sub(r'ApiResponse api_delete_member.*?return res;\n}', '', api_member_c, flags=re.DOTALL)
open('src/api/api_member.c', 'w', encoding='utf-8').write(api_member_c)

api_member_h = open('src/api/api_member.h', 'r', encoding='utf-8').read()
api_member_h = api_member_h.replace('ApiResponse api_delete_member(char *id);\n', '')
open('src/api/api_member.h', 'w', encoding='utf-8').write(api_member_h)

# 3. Fix sales.c (Content truncated...)
sales_c = open('src/core/sales.c', 'r', encoding='utf-8').read()
replacement = """    if (!(*details)) {
        fclose(file);
        *count = 0;
        return 0;
    }
    
    fseek(file, 0, SEEK_SET);
    int idx = 0;
    while (fgets(line, sizeof(line), file)) {
        char id[20];
        sscanf(line, "%[^,]", id);
        if (strcmp(id, transaction_id) == 0) {
            sscanf(line, "%[^,],%[^,],%[^,],%f,%d,%f", 
                (*details)[idx].transaction_id, 
                (*details)[idx].product_id, 
                (*details)[idx].product_name, 
                &(*details)[idx].price, 
                &(*details)[idx].quantity, 
                &(*details)[idx].subtotal);
            idx++;
        }
    }
    fclose(file);
    return 1;
}"""

sales_c = re.sub(r'\n    if \(\!\(\*details\)\).*', '\n' + replacement, sales_c, flags=re.DOTALL)
open('src/core/sales.c', 'w', encoding='utf-8').write(sales_c)

print("applied fixes")
