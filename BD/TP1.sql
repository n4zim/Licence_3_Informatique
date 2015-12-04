-- I) 1) --
CREATE TABLE EnsemblesAttributs (
  NumEnsAtt INTEGER,
  CONSTRAINT pk_EnsembleAttributs PRIMARY KEY (NumEnsAtt)
  ); 
  
 CREATE SEQUENCE NumEnsAtt; 
 
 CREATE TABLE EnsembleContientAttribut (
  NumEnsAtt INTEGER, 
  NomAtt VARCHAR(255),
  CONSTRAINT pk_EnsembleContientAttribut PRIMARY KEY (NumEnsAtt, NomAtt),
  CONSTRAINT fk_EnsContientAtt FOREIGN KEY (NumEnsAtt) REFERENCES EnsemblesAttributs(NumEnsAtt) ON DELETE CASCADE 
);

-- I) 2) -- 
create or replace FUNCTION CREERENSATTVIDE RETURN NUMBER IS 
  Val NUMBER;
BEGIN
  INSERT INTO EnsemblesAttributs VALUES (NumEnsAtt.NEXTVAL)
  RETURNING NumEnsAtt INTO Val;
  RETURN Val;
END CREERENSATTVIDE;


create or replace PROCEDURE AJOUTERATT 
(
  P_NOMATT IN VARCHAR2 , 
  P_NUMENSATT IN NUMBER
) AS 
BEGIN
  INSERT INTO EnsembleContientAttribut VALUES (P_NUMENSATT, P_NOMATT ) ;
END AJOUTERATT;

--------------------------------------------------------------

Variable VarNum NUMBER;
set SERVEROUTPUT ON;
Begin :VarNum := CREERENSATT('Premier , Second,Troisième'); end;
/
Print VarNum;
--select NOMATT from ENSEMBLECONTIENTATTRIBUT where NUMENSATT = :VarNum;
--Rollback;

--------------------------------------------------------------

Variable outputString VARCHAR2;
Begin :outputString := ENSATT2CHAINE(256064); end;
/
Print outputString;

--------------------------------------------------------------
