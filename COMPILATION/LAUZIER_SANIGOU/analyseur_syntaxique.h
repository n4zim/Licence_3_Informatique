void syntaxError(void);
n_prog * programme(void);
n_l_dec * optDecVariables(void);
n_l_dec * listeDecVariables(void);
n_l_dec * listeDecVariablesBis(void);
n_dec * declarationVariables(void);
n_dec * optTailleTableau(char * nom);
n_l_dec * listeDecFonctions(void);
n_dec * declarationFonction(void);
n_l_dec * listeParam(void);
n_l_dec * optListeDecVariables(void);
n_instr * instruction(void);
n_instr * instructionAffect(void);
n_instr * instructionBloc(void);
n_l_instr * listeInstructions(void);
n_instr * instructionSi(void);
n_instr * optSinon(void);
n_instr * instructionTantque(void);
n_instr * instructionAppel(void);
n_instr * instructionRetour(void);
n_instr * instructionEcriture(void);
n_instr * instructionVide(void);
n_exp * expression(void);
n_exp * expressionBis(n_exp * con);
n_exp * conjonction(void);
n_exp * conjonctionBis(n_exp * op1);
n_exp * negation(void);
n_exp * comparaison(void);
n_exp * comparaisonBis(n_exp * ex);
n_exp * expArith(void);
n_exp * expArithBis(n_exp * ter);
n_exp * terme(void);
n_exp * termeBis(n_exp * fac);
n_exp * facteur(void);
n_var * var(void);
n_exp * optIndice(void);
n_appel * appelFct(void);
n_l_exp * listeExpressions(void);
n_l_exp * listeExpressionsBis(void);
void analyseur_syntaxique(int uc);