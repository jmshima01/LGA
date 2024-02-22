import sys

GRAMMAR_DICT: dict[str, list[list[str]]] = {}
NON_TERMINAL_SET: set[str] = set()
SYMBOL_SET: set[str] = set()
START_SYMBOL: str = ''

# Dummy method for derives to lambda
def derives_to_lambda(non_terminal: str, T: list):
    if (non_terminal == 'startGoal'): return True
    if (non_terminal == 'A'): return True
    if (non_terminal == 'Var'): return True
    return False

# Dummy method for first set
def first_set(check_list: list[str], T: set):
    if (check_list[0] == 'startGoal' or check_list[0] == 'A' or check_list == 'T' or check_list[0] == 'Var'): return {'e', 'f', 'g', 'h', 'i', 'j', 'k'}
    if (check_list[0] == 'E'): return {'a', 's', 'zero', 'e', 'f', 'g', 'h', 'i', 'j', 'k'}
    return check_list


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
    print(f'\nGrammar Start Symbol or Goal: {START_SYMBOL}')

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


if __name__ == '__main__':
    main()