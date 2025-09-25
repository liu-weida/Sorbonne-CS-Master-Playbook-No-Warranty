from mersenne_twister import *
import operator
import functools
def XOR(*seqs):
    """
    XOR toghether an arbitrary number of bytes()
    """
    return bytearray([functools.reduce(operator.xor, t, 0) for t in zip(*seqs)])

with open("/Users/yijiezheng/Desktop/my_RML/my_RML.txt","r") as f:
	cipher = f.read()
cipher = '\n'.join(cipher.split('\n')[1:-5])

plaintext = """# RML GRAMMAR
# ===========
# 
# <statements> ::= <empty> | <statement> | <statement> . <statements>
#     
# <statement> ::= extern type <id>
#             | let <id> : <type> = <expression>
#             | extern def <id> ( <formals> ) -> <type> 
#             | def <id> ( <formals> ) -> <type> <code>
#     
# <formals> ::= <empty> | <id> : <type> | <id> : <type> , <formals>
#     
# <type> ::= <id> | <type> list | <type> maybe | anything | something | nothing
#     
# <code> ::= { instructions }
#    
# <instructions> ::= <empty> | <instruction> | <instruction> ; <instructions>
#    
# <instruction> ::= <expression>
#                 | let <id> : <type> = <expression>
#                 | <id> = <expression>
#                 | for <id> in <expression> <code>
#                 | if <expression> <code>
#                 | if <expression> <code else <code>
#                 | case <expression> | None -> <code> | Some <id> -> <code>
#                 | return <expression>
#                 | return
#                 | panic
#     
# <expressions> ::= <empty> | <expression> | <expression> , <expressions>
# 
# <expression> ::= None 
#              | <string>
#              | <int>
#              | [ <expression> ]                               --- list
#              | not <expression>
#              | <expression> <operator> <expression>
#              | <id> ( <expressions> )                         --- function call
#              | ( <expression> )
# 
# <operator> ::= + | - | * | / | == | != | < | > | or | and | :: | ^ 
# 
# -------------------------------------------------------------------------------
# 
# RML TYPE SYSTEM
# ===============
# 
# RML is strongly typed, and the type discipline is checked statically at 
# compile-time. The special value None has type "nothing" (it denotes the absence
# of value). We have the following rules:
# 
# For all types R and S, 
# 
#     "anything" is a subtype of R,
#     R          is a subtype of "something",
#     "nothing"  is a subtype of "R maybe",
#     R          is a subtype of "R maybe",
#     "R list"   is a subtype of "S list"      iff    R is a subtype of S,
#     "R maybe"  is a subtype of "S maybe"     iff    R is a subtype of S.
# 
# 
# (Recall that S is a subtype of T if a value of type S can always be used in 
# place of a value of type T). Keen observers will have noticed that RML uses 
# non-structural subtyping.
# 
# The programmer must declare the type of all variables (using the "let" 
# instruction). New values can simply be assigned to already-declared variables.
# When a value is assigned to a variable, it must have a compatible type.
# 
# The base types are string, data, int and bool. The + operator works with ints, 
# strings, data and list (it concatenates them). The -, *, /, <, > operators
# require int arguments. The equality comparison operator == and != work with any 
# type. The :: operator (which appends an item at the end of a list) requires a 
# list and a compatible item. The ^ operator (XOR) only works on data. 
# 
# In other terms:
# 
#      +  : (int, int) -> int
#         | (string, string) -> string
#         | (data, data) -> data
#         | (R list, S list) -> R list    iff S is a subtype of R
#         | (R list, S list) -> S list    iff R is a subtype of S
# 
# -, *, / : (int, int) -> int
#       ^ : (data, data) -> data
#     ::  : (R list, S) -> R list         iff S a subtype of R
# 
#    <, > : (int, int) -> bool
# and, or : (bool, bool) -> bool
#     not : (bool) -> bool  
#  ==, != : (something, something) -> bool
# 
# The type system checks that arguments have compatible types in function calls,
# and that the return values of function have a compatible type. Without argument,
# the "return" statement is equivalent to "return None". The special "panic" 
# statement is equivalent to a "return" with a special value of type "anything".
#     
# The "case" statement is specifically designed to deal with "R maybe" values ; in
# the "Some <id>" branch, the name <id> is bound to a value of type R, which is 
# thus guaranteed not to be None.
# 
# -------------------------------------------------------------------------------
#""".encode()

t = MersenneTwisterCipher()

cipher = cipher.replace(' ','').replace('\n','')
cipher = bytes.fromhex(cipher)

print("Size cipher check : ", len(cipher) == 17553)

mask = bytes(XOR(plaintext,cipher))
MT = []

for i in range(2496):
	if (i % 4 == 0):
		x = t.inv_f(int.from_bytes(mask[i:i+4],"little"))
		MT.append(x) # little

print(t.encrypt2(cipher,MT).decode())