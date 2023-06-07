# AVL Tree
## Loop vs Recusrion realisation methods
### The loop method is better than recursion.
* [<span style="color:green">+</span>] No stack overflow with big number of items.
* [<span style="color:green">+</span>] Lower performance costs than the recursion method.
* [<span style="color:red">-</span>] It uses an extra field in node for store a link to parent.

### The recursion method is bad:
* [<span style="color:red">-</span>] It is the cause of stack overflow with big number of items.
ERROR_CODE: (NTSTATUS) 0xc00000fd - A new guard page for the stack cannot be created.
* [<span style="color:red">-</span>] It requires additional performance costs for calling the functions of forming a new function frame and placing the return address in the stack.
