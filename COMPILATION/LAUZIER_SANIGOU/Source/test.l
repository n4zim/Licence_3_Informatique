<programme>
  <optDecVariables>
    <listeDecVariables>
      <declarationVariable>
        <mot_clef>entier</mot_clef>
        <id_variable>$c</id_variable>
        <optTailleTableau>
        </optTailleTableau>
      </declarationVariable>
      <listeDecVariablesBis>
      </listeDecVariablesBis>
    </listeDecVariables>
    <symbole>POINT_VIRGULE</symbole>
  </optDecVariables>
  <listeDecFonctions>
    <declarationFonction>
      <id_fonction>initialiser</id_fonction>
      <listeParam>
        <symbole>PARENTHESE_OUVRANTE</symbole>
        <optListeDecVariables>
        </optListeDecVariables>
        <symbole>PARENTHESE_FERMANTE</symbole>
      </listeParam>
      <optDecVariables>
      </optDecVariables>
      <instructionBloc>
        <symbole>ACCOLADE_OUVRANTE</symbole>
        <listeInstructions>
          <instruction>
            <instructionAffect>
              <var>
                <id_variable>$c</id_variable>
                <optIndice>
                </optIndice>
              </var>
              <symbole>EGAL</symbole>
              <expression>
                <conjonction>
                  <negation>
                    <comparaison>
                      <expArith>
                        <terme>
                          <facteur>
                            <nombre>0</nombre>
                          </facteur>
                          <termeBis>
                          </termeBis>
                        </terme>
                        <expArithBis>
                        </expArithBis>
                      </expArith>
                      <comparaisonBis>
                      </comparaisonBis>
                    </comparaison>
                  </negation>
                  <conjonctionBis>
                  </conjonctionBis>
                </conjonction>
                <expressionBis>
                </expressionBis>
              </expression>
              <symbole>POINT_VIRGULE</symbole>
            </instructionAffect>
          </instruction>
          <listeInstructions>
          </listeInstructions>
        </listeInstructions>
        <symbole>ACCOLADE_FERMANTE</symbole>
      </instructionBloc>
    </declarationFonction>
    <listeDecFonctions>
      <declarationFonction>
        <id_fonction>afficher</id_fonction>
        <listeParam>
          <symbole>PARENTHESE_OUVRANTE</symbole>
          <optListeDecVariables>
            <listeDecVariables>
              <declarationVariable>
                <mot_clef>entier</mot_clef>
                <id_variable>$n</id_variable>
                <optTailleTableau>
                </optTailleTableau>
              </declarationVariable>
              <listeDecVariablesBis>
              </listeDecVariablesBis>
            </listeDecVariables>
          </optListeDecVariables>
          <symbole>PARENTHESE_FERMANTE</symbole>
        </listeParam>
        <optDecVariables>
          <listeDecVariables>
            <declarationVariable>
              <mot_clef>entier</mot_clef>
              <id_variable>$i</id_variable>
              <optTailleTableau>
              </optTailleTableau>
            </declarationVariable>
            <listeDecVariablesBis>
            </listeDecVariablesBis>
          </listeDecVariables>
          <symbole>POINT_VIRGULE</symbole>
        </optDecVariables>
        <instructionBloc>
          <symbole>ACCOLADE_OUVRANTE</symbole>
          <listeInstructions>
            <instruction>
              <instructionAffect>
                <var>
                  <id_variable>$i</id_variable>
                  <optIndice>
                  </optIndice>
                </var>
                <symbole>EGAL</symbole>
                <expression>
                  <conjonction>
                    <negation>
                      <comparaison>
                        <expArith>
                          <terme>
                            <facteur>
                              <nombre>0</nombre>
                            </facteur>
                            <termeBis>
                            </termeBis>
                          </terme>
                          <expArithBis>
                          </expArithBis>
                        </expArith>
                        <comparaisonBis>
                        </comparaisonBis>
                      </comparaison>
                    </negation>
                    <conjonctionBis>
                    </conjonctionBis>
                  </conjonction>
                  <expressionBis>
                  </expressionBis>
                </expression>
                <symbole>POINT_VIRGULE</symbole>
              </instructionAffect>
            </instruction>
            <listeInstructions>
              <instruction>
                <instructionTantque>
                  <mot_clef>tantque</mot_clef>
                  <expression>
                    <conjonction>
                      <negation>
                        <comparaison>
                          <expArith>
                            <terme>
                              <facteur>
                                <symbole>PARENTHESE_OUVRANTE</symbole>
                                <expression>
                                  <conjonction>
                                    <negation>
                                      <comparaison>
                                        <expArith>
                                          <terme>
                                            <facteur>
                                              <var>
                                                <id_variable>$i</id_variable>
                                                <optIndice>
                                                </optIndice>
                                              </var>
                                            </facteur>
                                            <termeBis>
                                            </termeBis>
                                          </terme>
                                          <expArithBis>
                                          </expArithBis>
                                        </expArith>
                                        <comparaisonBis>
                                          <symbole>INFERIEUR</symbole>
                                          <expArith>
                                            <terme>
                                              <facteur>
                                                <var>
                                                  <id_variable>$n</id_variable>
                                                  <optIndice>
                                                  </optIndice>
                                                </var>
                                              </facteur>
                                              <termeBis>
                                              </termeBis>
                                            </terme>
                                            <expArithBis>
                                            </expArithBis>
                                          </expArith>
                                          <comparaisonBis>
                                          </comparaisonBis>
                                        </comparaisonBis>
                                      </comparaison>
                                    </negation>
                                    <conjonctionBis>
                                    </conjonctionBis>
                                  </conjonction>
                                  <expressionBis>
                                  </expressionBis>
                                </expression>
                                <symbole>PARENTHESE_FERMANTE</symbole>
                              </facteur>
                              <termeBis>
                              </termeBis>
                            </terme>
                            <expArithBis>
                            </expArithBis>
                          </expArith>
                          <comparaisonBis>
                          </comparaisonBis>
                        </comparaison>
                      </negation>
                      <conjonctionBis>
                      </conjonctionBis>
                    </conjonction>
                    <expressionBis>
                    </expressionBis>
                  </expression>
                  <mot_clef>faire</mot_clef>
                  <instructionBloc>
                    <symbole>ACCOLADE_OUVRANTE</symbole>
                    <listeInstructions>
                      <instruction>
                        <instructionEcriture>
                          <mot_clef>ecrire</mot_clef>
                          <symbole>PARENTHESE_OUVRANTE</symbole>
                          <expression>
                            <conjonction>
                              <negation>
                                <comparaison>
                                  <expArith>
                                    <terme>
                                      <facteur>
                                        <var>
                                          <id_variable>$i</id_variable>
                                          <optIndice>
                                          </optIndice>
                                        </var>
                                      </facteur>
                                      <termeBis>
                                      </termeBis>
                                    </terme>
                                    <expArithBis>
                                    </expArithBis>
                                  </expArith>
                                  <comparaisonBis>
                                  </comparaisonBis>
                                </comparaison>
                              </negation>
                              <conjonctionBis>
                              </conjonctionBis>
                            </conjonction>
                            <expressionBis>
                            </expressionBis>
                          </expression>
                          <symbole>PARENTHESE_FERMANTE</symbole>
                          <symbole>POINT_VIRGULE</symbole>
                        </instructionEcriture>
                      </instruction>
                      <listeInstructions>
                        <instruction>
                          <instructionAffect>
                            <var>
                              <id_variable>$i</id_variable>
                              <optIndice>
                              </optIndice>
                            </var>
                            <symbole>EGAL</symbole>
                            <expression>
                              <conjonction>
                                <negation>
                                  <comparaison>
                                    <expArith>
                                      <terme>
                                        <facteur>
                                          <var>
                                            <id_variable>$i</id_variable>
                                            <optIndice>
                                            </optIndice>
                                          </var>
                                        </facteur>
                                        <termeBis>
                                        </termeBis>
                                      </terme>
                                      <expArithBis>
                                        <symbole>PLUS</symbole>
                                        <terme>
                                          <facteur>
                                            <nombre>1</nombre>
                                          </facteur>
                                          <termeBis>
                                          </termeBis>
                                        </terme>
                                        <expArithBis>
                                        </expArithBis>
                                      </expArithBis>
                                    </expArith>
                                    <comparaisonBis>
                                    </comparaisonBis>
                                  </comparaison>
                                </negation>
                                <conjonctionBis>
                                </conjonctionBis>
                              </conjonction>
                              <expressionBis>
                              </expressionBis>
                            </expression>
                            <symbole>POINT_VIRGULE</symbole>
                          </instructionAffect>
                        </instruction>
                        <listeInstructions>
                        </listeInstructions>
                      </listeInstructions>
                    </listeInstructions>
                    <symbole>ACCOLADE_FERMANTE</symbole>
                  </instructionBloc>
                </instructionTantque>
              </instruction>
              <listeInstructions>
              </listeInstructions>
            </listeInstructions>
          </listeInstructions>
          <symbole>ACCOLADE_FERMANTE</symbole>
        </instructionBloc>
      </declarationFonction>
      <listeDecFonctions>
        <declarationFonction>
          <id_fonction>incrementer</id_fonction>
          <listeParam>
            <symbole>PARENTHESE_OUVRANTE</symbole>
            <optListeDecVariables>
            </optListeDecVariables>
            <symbole>PARENTHESE_FERMANTE</symbole>
          </listeParam>
          <optDecVariables>
          </optDecVariables>
          <instructionBloc>
            <symbole>ACCOLADE_OUVRANTE</symbole>
            <listeInstructions>
              <instruction>
                <instructionAffect>
                  <var>
                    <id_variable>$c</id_variable>
                    <optIndice>
                    </optIndice>
                  </var>
                  <symbole>EGAL</symbole>
                  <expression>
                    <conjonction>
                      <negation>
                        <comparaison>
                          <expArith>
                            <terme>
                              <facteur>
                                <var>
                                  <id_variable>$c</id_variable>
                                  <optIndice>
                                  </optIndice>
                                </var>
                              </facteur>
                              <termeBis>
                              </termeBis>
                            </terme>
                            <expArithBis>
                              <symbole>PLUS</symbole>
                              <terme>
                                <facteur>
                                  <nombre>1</nombre>
                                </facteur>
                                <termeBis>
                                </termeBis>
                              </terme>
                              <expArithBis>
                              </expArithBis>
                            </expArithBis>
                          </expArith>
                          <comparaisonBis>
                          </comparaisonBis>
                        </comparaison>
                      </negation>
                      <conjonctionBis>
                      </conjonctionBis>
                    </conjonction>
                    <expressionBis>
                    </expressionBis>
                  </expression>
                  <symbole>POINT_VIRGULE</symbole>
                </instructionAffect>
              </instruction>
              <listeInstructions>
              </listeInstructions>
            </listeInstructions>
            <symbole>ACCOLADE_FERMANTE</symbole>
          </instructionBloc>
        </declarationFonction>
        <listeDecFonctions>
          <declarationFonction>
            <id_fonction>main</id_fonction>
            <listeParam>
              <symbole>PARENTHESE_OUVRANTE</symbole>
              <optListeDecVariables>
              </optListeDecVariables>
              <symbole>PARENTHESE_FERMANTE</symbole>
            </listeParam>
            <optDecVariables>
            </optDecVariables>
            <instructionBloc>
              <symbole>ACCOLADE_OUVRANTE</symbole>
              <listeInstructions>
                <instruction>
                  <instructionAppel>
                    <appelFct>
                      <id_fonction>initialiser</id_fonction>
                      <symbole>PARENTHESE_OUVRANTE</symbole>
                      <listeExpressions>
                      </listeExpressions>
                      <symbole>PARENTHESE_FERMANTE</symbole>
                    </appelFct>
                    <symbole>POINT_VIRGULE</symbole>
                  </instructionAppel>
                </instruction>
                <listeInstructions>
                  <instruction>
                    <instructionAppel>
                      <appelFct>
                        <id_fonction>incrementer</id_fonction>
                        <symbole>PARENTHESE_OUVRANTE</symbole>
                        <listeExpressions>
                        </listeExpressions>
                        <symbole>PARENTHESE_FERMANTE</symbole>
                      </appelFct>
                      <symbole>POINT_VIRGULE</symbole>
                    </instructionAppel>
                  </instruction>
                  <listeInstructions>
                    <instruction>
                      <instructionAppel>
                        <appelFct>
                          <id_fonction>afficher</id_fonction>
                          <symbole>PARENTHESE_OUVRANTE</symbole>
                          <listeExpressions>
                            <expression>
                              <conjonction>
                                <negation>
                                  <comparaison>
                                    <expArith>
                                      <terme>
                                        <facteur>
                                          <var>
                                            <id_variable>$c</id_variable>
                                            <optIndice>
                                            </optIndice>
                                          </var>
                                        </facteur>
                                        <termeBis>
                                        </termeBis>
                                      </terme>
                                      <expArithBis>
                                      </expArithBis>
                                    </expArith>
                                    <comparaisonBis>
                                    </comparaisonBis>
                                  </comparaison>
                                </negation>
                                <conjonctionBis>
                                </conjonctionBis>
                              </conjonction>
                              <expressionBis>
                              </expressionBis>
                            </expression>
                            <listeExpressionsBis>
                            </listeExpressionsBis>
                          </listeExpressions>
                          <symbole>PARENTHESE_FERMANTE</symbole>
                        </appelFct>
                        <symbole>POINT_VIRGULE</symbole>
                      </instructionAppel>
                    </instruction>
                    <listeInstructions>
                    </listeInstructions>
                  </listeInstructions>
                </listeInstructions>
              </listeInstructions>
              <symbole>ACCOLADE_FERMANTE</symbole>
            </instructionBloc>
          </declarationFonction>
          <listeDecFonctions>
          </listeDecFonctions>
        </listeDecFonctions>
      </listeDecFonctions>
    </listeDecFonctions>
  </listeDecFonctions>
</programme>
