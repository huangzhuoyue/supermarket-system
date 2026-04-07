import os

def remove_and_append(source_file, target_file, keyword_starts):
    with open(source_file, 'r', encoding='utf-8') as f:
        lines = f.readlines()
    
    source_lines = []
    target_lines = []
    
    in_target_block = False
    
    for i, line in enumerate(lines):
        # Check if line indicates start of a block to move
        # We also want to move the /** ... */ comment blocks above them.
        # But this is a bit tricky. We'll simply find the block and not worry too much about comments if it's too complex,
        # checking backwards for `/**`
        
        # We can just match the function signatures. We've seen them earlier.
        is_start = any(line.startswith(start) for start in keyword_starts)
        
        if is_start:
            in_target_block = True
            # backtrack to grab comments
            j = len(source_lines) - 1
            while j >= 0 and (source_lines[j].strip().startswith('*') or source_lines[j].strip().startswith('/*')):
                target_lines.insert(0, source_lines.pop(j))
                j -= 1
        
        if in_target_block:
            target_lines.append(line)
            if line.startswith('}'):
                in_target_block = False
        else:
            source_lines.append(line)
            
    with open(source_file, 'w', encoding='utf-8') as f:
        f.writelines(source_lines)
        
    with open(target_file, 'a', encoding='utf-8') as f:
        f.write('\n')
        f.writelines(target_lines)

# product
remove_and_append(
    'src/core/product.c', 
    'src/cli/product_ui.c', 
    ['void displayProduct(', 'void displayProductList(', 'void displayAllProducts(', 'void checkLowStock(']
)

# sales
remove_and_append(
    'src/core/sales.c',
    'src/cli/sales_ui.c',
    ['void displayCart(', 'void displayTransaction(']
)

# member
remove_and_append(
    'src/core/member.c',
    'src/cli/member_ui.c',
    ['void displayMember(', 'void displayMemberList(', 'void displayAllMembers(']
)

print("Done extracting UI functions from core.")
