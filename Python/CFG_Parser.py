import sys

GRAMMAR_DICT: dict[str, list[list[str]]] = {}
NON_TERMINAL_SET: set[str] = set()
SYMBOL_SET: set[str] = set()
START_SYMBOL: str = ''


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
    F = set()
    if(front not in T):
        T.add(front)
        for production in GRAMMAR_DICT[front]:
            if production[0] == "lambda":
                continue
            G, I = first_set_full(production, set([v for v in T]))
            F = F.union(G)
    if derives_to_lambda(front) and len(rest):
        G, I = first_set_full(rest, set([v for v in T]))
        F = F.union(G)
    return F,T

# Overload for first sets 
def first_set(check_list: list[str]):
    return first_set_full(check_list,set())

def follow_set(A: str, T:set):
    if A in T:
        return set(), T
    T.add(A)
    F = set()
    for p in GRAMMAR_DICT:
        for a in len(p):
            if p[a] == A:
                if a < len(p) - 1:
                    G,I = first_set(p[a+1:])
                    F.union(G)
                derives = True
                for b in p[a+1:]:
                    print('or')
                    if not derives_to_lambda(b):
                        derives = False
                        break
                if a == len(p) - 1 or (derives and p != GRAMMAR_DICT[START_SYMBOL]):
                    G,I = follow_set(GRAMMAR_DICT.index(p))
                    F.union(G)
        print('here')
    return F,T


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
