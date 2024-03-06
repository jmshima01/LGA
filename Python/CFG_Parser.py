import sys

GRAMMAR_DICT: dict[str, list[list[str]]] = {}
NON_TERMINAL_SET: set[str] = set()
SYMBOL_SET: set[str] = set()
START_SYMBOL: str = ''

LL1_PARSE_TABLE: dict[str, dict[str, tuple[str,int]]] = {}

# Method for derives to lambda
def derives_to_lambda(non_terminal: str, T: list=[]):
    for production in GRAMMAR_DICT[non_terminal]:
        if production in T:
            continue
        if production[0] == "lambda":
            return True
        if not all(map(lambda val: val in NON_TERMINAL_SET, production)):
            continue
        deriveToLambda = lambda non_term: derives_to_lambda(non_term, T+[production])
        if(all(map(deriveToLambda, production))):
            return True
    return False

# Method for first set
def first_set_full(check_list: list[str], T: set):
    front,rest = check_list[0],check_list[1:]
    if front in SYMBOL_SET and front not in NON_TERMINAL_SET:
        return set([front]), T
    first = set()
    if(front not in T):
        T.add(front)
        for production in GRAMMAR_DICT[front]:
            if production[0] == "lambda":
                continue
            G, I = first_set_full(production, set([v for v in T]))
            first = first.union(G)
    if derives_to_lambda(front) and len(rest):
        G, I = first_set_full(rest, set([v for v in T]))
        first = first.union(G)
    return first,T

# Overload for first sets 
def first_set(check_list: list[str]):
    return first_set_full(check_list,set())

# Method for follow sets
def follow_set_full(non_terminal: str, T:set):
    if non_terminal in T:
        return set(), T
    T.add(non_terminal)
    follow = set()
    # lots of unpacking, sucks
    for non_term in NON_TERMINAL_SET:
        for production in GRAMMAR_DICT[non_term]:
            for index, sym in enumerate(production):
                if sym == non_terminal:
                    phi = production[index+1:]
                    if len(phi):
                        G,I = first_set(phi)
                        follow = follow.union(G)
                    
                    # Simple boolean functions
                    all_non_term = lambda : all(map(lambda val: val in NON_TERMINAL_SET, phi))
                    all_derive_lambda = lambda : all(map(lambda val: derives_to_lambda(val), phi))

                    if not len(phi) or (all_non_term() and all_derive_lambda()):
                        G,I = follow_set_full(non_term, set([v for v in T]))
                        follow = follow.union(G)
    return follow,T

# Overload for follow sets
def follow_set(non_terminal: str):
    return follow_set_full(non_terminal, set())

# factors out common prefixes, added terminals are also factored
def factor_rules():
    # helper method, does longest common prefix for a list of rules
    def longestCommonPrefix(rules: list[list[str]]):
        prefix = [] # storage for prefix
        for i in range(min(rules,len)): # cheat to save time
            nextSym = rules[0][i] # next sym that could be added
            for r in range(0,len(rules)):
                if nextSym != rules[r][i]:
                    return prefix # return on first failed match
            prefix.append(nextSym)
        return prefix
    
    # this is this functions replacement for the NON_TERMINAL_SET
    non_term_list = list(NON_TERMINAL_SET)
    # helper method, adds new non-terminals to the grammer
    def add_non_terminal():
        new_non_term = "A"
        while new_non_term in SYMBOL_SET:
            new_non_term+="A" #easiest way of doing this
        # add new symbol to everthing that needs it
        SYMBOL_SET.add(new_non_term)
        NON_TERMINAL_SET.add(new_non_term)
        GRAMMAR_DICT[new_non_term] = []
        non_term_list.append(new_non_term)
        return new_non_term
    
    # (might need to swap this with a while loop)
    # reason for non_term_list is so that when new non-terms are added,
    # they get pushed to the end and get cleaned later
    for non_term in non_term_list:
        comm_prefix = longestCommonPrefix(GRAMMAR_DICT[non_term])
        while len(comm_prefix):
            nt_V = add_non_terminal()
            for production in GRAMMAR_DICT[non_term]:
                # split production into its prefix and beta
                beta = production[len(comm_prefix):]
                # lambda split should probably refactor
                if len(beta): 
                    GRAMMAR_DICT[nt_V].append(beta)
                else:
                    GRAMMAR_DICT[nt_V].append("lambda")
            # reset old rule
            comm_prefix.append(nt_V)
            GRAMMAR_DICT[non_term] = [comm_prefix]
            comm_prefix = longestCommonPrefix(GRAMMAR_DICT[non_term])

# Dummy method for predict sets
def predict_set(rule: tuple[str, list[str]]):
    return set("a","b","c")

# Build LL(1) parse table
def build_parse_table():
    # get set of terminal symbols
    terminal_set = set()
    for symbol in SYMBOL_SET:
        if symbol not in NON_TERMINAL_SET:
            terminal_set.add(symbol)
    
    for non_term in NON_TERMINAL_SET:
        # initilize each element of "row" to None 
        LL1_PARSE_TABLE[non_term] = {symbol:None for symbol in terminal_set}
        # for each rule add the terminals in its predict set to the "row"
        for r_idx, production in enumerate(GRAMMAR_DICT[non_term]):
            rule = (non_term,production)
            for terminal in predict_set(rule):
                LL1_PARSE_TABLE[non_term][terminal] = (non_term,r_idx)

def parse_input(input_file_name):
    global GRAMMAR_DICT, NON_TERMINAL_SET, SYMBOL_SET, START_SYMBOL

    try:
        with open(input_file_name, '+r') as input_file:
            active_non_terminal = ''
            alternators_met_for_active = 0 # This is used for separating the rules for the respective 
            for line in input_file:
                parts = line.strip().split(' ')

                rhs_operator_met = False
                for part_str in parts:
                    if (part_str == "->" or part_str == "|"):
                        if (part_str == "|"):
                            alternators_met_for_active += 1
                            GRAMMAR_DICT[active_non_terminal].append([])
                        rhs_operator_met = True
                        continue

                    if not rhs_operator_met: # If an RHS operator hasn't meen met,
                        if (active_non_terminal != part_str): # If LHS is the not the same, 
                            active_non_terminal = part_str # set the active non_terminal 
                            alternators_met_for_active = 0 # reset alternator count
                        else:
                            alternators_met_for_active += 1
                            GRAMMAR_DICT[active_non_terminal].append([])

                        NON_TERMINAL_SET.add(part_str) # add the LHS str to the non-terminal set and 
                        SYMBOL_SET.add(part_str) # add it to the non_terminal set

                        if part_str not in GRAMMAR_DICT.keys(): # and create an entry in the dict representing the grammar while also setting the active non terminal
                            GRAMMAR_DICT[part_str] = [[]]

                    else: # If an RHS operator has been met, 
                        if part_str != "lambda": # if the part isn't lambda, 
                            SYMBOL_SET.add(part_str) # add it to the non_terminal set

                        if part_str == "$": # Set the start symbol if '$' is found
                            START_SYMBOL = active_non_terminal

                        # Regardless of lambda or not, add the symbol to the current active LHS's most recent respective symbol list
                        GRAMMAR_DICT[active_non_terminal][alternators_met_for_active].append(part_str)
    except FileNotFoundError:
        print("File does not exist")
        exit(-1) 

def print_grammer_nonterminals():
    print(', '.join(NON_TERMINAL_SET))

def print_grammar_terminals():
    print(', '.join(SYMBOL_SET))

def print_grammar_symbols():
    print(', '.join(SYMBOL_SET.union(NON_TERMINAL_SET)))

def print_grammar_rules_verbose():
    rule_counter = 1
    for lhs, rhs_list in GRAMMAR_DICT.items():
        for rhs_sublist in rhs_list:
            print(f'({rule_counter})   {lhs} -> {" ".join(rhs_sublist)}')
            rule_counter += 1

def print_grammar_full():
    print('Grammar Non-Terminals')
    print_grammer_nonterminals()
    print('Grammar Symbols')
    print_grammar_terminals()
    print('\nGrammar Rules')
    print_grammar_rules_verbose()
    print(f'\nGrammar Start Symbol or Goal: {START_SYMBOL}\n')

def print_derives_to_lambda():
    for non_terminal in NON_TERMINAL_SET:
        print(f'{non_terminal} derives to lambda: {derives_to_lambda(non_terminal)}')
    print()

def print_first_sets():
    for non_terminal in NON_TERMINAL_SET:
        print(f'First Set of {non_terminal}: {first_set([non_terminal])}')
    print()

def print_follow_sets():
    for non_terminal in NON_TERMINAL_SET:
        print(f'Follow Set of {non_terminal}: {follow_set(non_terminal)}')
    print()

def main():
    # Handle command line arguments
    if len(sys.argv) != 2:
        print(f'Correct usage: {__file__} <input_file.cfg>')
        exit(-1)

    input_file_name = sys.argv[1]

    # Parse the input file into the global variables
    parse_input(input_file_name)

    # Print out the required stuff
    print_grammar_full()

    # Print out derivations to lambda
    print_derives_to_lambda()

    # Print first sets 
    print_first_sets()

    print_follow_sets()


if __name__ == '__main__':
    main()
