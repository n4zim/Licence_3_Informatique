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
-- I) 3) --
CREATE OR REPLACE FUNCTION ESTELEMENT 
(
  P_NOMATT IN VARCHAR2 
, P_NUMENSATT IN NUMBER 
) RETURN INTEGER AS 
resultString VARCHAR(2000);
BEGIN

SELECT NOMATT INTO resultString
FROM ENSEMBLECONTIENTATTRIBUT
WHERE  NUMENSATT = p_NUMENSATT AND NOMATT = P_NOMATT; 

IF(resultString IS NOT NULL) THEN 
  RETURN 1; 
END IF;
  RETURN 0;

END ESTELEMENT;

--------------------------------------------------------------

SELECT ESTELEMENT('Premier',256064) FROM DUAL;

--------------------------------------------------------------

CREATE OR REPLACE FUNCTION ESTINCLUS 
(
  P_NUMENSATT_1 IN NUMBER 
, P_NUMENSATT_2 IN NUMBER 
) RETURN INTEGER AS
CURSOR curseur IS SELECT * FROM ENSEMBLECONTIENTATTRIBUT WHERE NUMENSATT = P_NUMENSATT_1; 
R1 NUMBER;
R2 VARCHAR(2000);
BEGIN

OPEN curseur; 
LOOP 
  FETCH curseur INTO R1, R2;
  EXIT WHEN curseur%NOTFOUND;
  IF(ESTELEMENT(R2,P_NUMENSATT_2) = 0) THEN RETURN 0; END IF;
END LOOP;

  RETURN 1;
END ESTINCLUS;

--------------------------------------------------------------

SELECT ESTINCLUS(256063, 256064) FROM DUAL; 

--------------------------------------------------------------

CREATE OR REPLACE FUNCTION UNIONATT 
(
  P_NUMENSATT1 IN NUMBER 
, P_NUMENSATT2 IN NUMBER 
) RETURN NUMBER AS 
STR VARCHAR(2000);
ENS NUMBER;
BEGIN

  STR := ENSATT2CHAINE(P_NUMENSATT1)||','||ENSATT2CHAINE(P_NUMENSATT2);
  ENS := CREERENSATT(STR);

  RETURN ENS;
END UNIONATT;

--------------------------------------------------------------
-- II) 1) --
CREATE TABLE DFs(
  NumDF INTEGER,
  NumEnsGauche INTEGER NOT NULL,
  NumEnsDroit INTEGER NOT NULL,
  CONSTRAINT pk_DFs PRIMARY KEY (NumDF),
  CONSTRAINT u_DFs UNIQUE (NumEnsGauche, NumEnsDroit),
  CONSTRAINT fk_NumEnsGauche_DFs FOREIGN KEY (NumEnsGauche) REFERENCES EnsemblesAttributs(NumEnsAtt),
  CONSTRAINT fk_NumEnsDroit_DFs FOREIGN KEY (NumEnsDroit) REFERENCES EnsemblesAttributs(NumEnsAtt)
);

CREATE SEQUENCE NumDF;

--------------------------------------------------------------

CREATE TABLE EnsemblesDFs(
  NumEnsDF INTEGER,
  CONSTRAINT pk_EnsDFs PRIMARY KEY (NumEnsDF)
);

--------------------------------------------------------------

CREATE TABLE EnsembleContientDF(
  NumEnsDF INTEGER,
  NumDF INTEGER,
  CONSTRAINT pk_EnsContDF PRIMARY KEY (NumEnsDF, NumDF),
  CONSTRAINT fk_NumEnsDF_EnsContDF FOREIGN KEY (NumEnsDF) REFERENCES EnsemblesDFs(NumEnsDF) ON DELETE CASCADE,
  CONSTRAINT fk_NumDF_EnsContDF FOREIGN KEY (NumDF) REFERENCES DFs (NumDF)
);

--------------------------------------------------------------

CREATE SEQUENCE NumEnsDF;

CREATE OR REPLACE FUNCTION CREERENSATTVIDE RETURN NUMBER IS
varIteration NUMBER(3);
BEGIN
  INSERT INTO EnsemblesAttributs VALUES (NumEnsAtt.NextVal)
  RETURNING NumEnsAtt INTO varIteration;
  RETURN varIteration;
END;

--------------------------------------------------------------


declare Num NUMBER(3);
Begin Num := CREERENSATT('C,A'); end;

declare Num NUMBER(3);
Begin Num := CREERDF('C,A,H->C,H'); end;

--------------------------------------------------------------

create or replace FUNCTION INTERSECTIONATT 
(
  P_NUMENSATT_1 IN NUMBER 
, P_NUMENSATT_2 IN NUMBER 
) RETURN NUMBER IS
CURSOR curseur IS
  SELECT* FROM ENSEMBLECONTIENTATTRIBUT WHERE NUMENSATT = P_NUMENSATT_1;
NumEnsTmp  NUMBER;
NomAttTmp VARCHAR2(255);
Num NUMBER;
BEGIN
  Num := CREERENSATTVIDE();
  OPEN curseur;
  LOOP
    FETCH curseur INTO NumEnsTmp,NomAttTmp;
    EXIT WHEN curseur%NOTFOUND;
    IF ESTELEMENT(NomAttTmp, P_NUMENSATT_2)=1 THEN AJOUTERATT(NomAttTmp,Num);
    END IF;
  END LOOP;
  RETURN Num;
END INTERSECTIONATT;

--------------------------------------------------------------

create or replace FUNCTION SOUSTRACTIONATT 
(
  P_NUMENSATT_1 IN NUMBER 
, P_NUMENSATT_2 IN NUMBER 
) RETURN NUMBER IS 
CURSOR curseur IS
  SELECT* FROM ENSEMBLECONTIENTATTRIBUT WHERE NUMENSATT = P_NUMENSATT_1;
NumEnsTmp  NUMBER;
NomAttTmp VARCHAR2(255);
Num NUMBER;
BEGIN
  Num := CREERENSATTVIDE();
  OPEN curseur;
  LOOP
    FETCH curseur INTO NumEnsTmp,NomAttTmp;
    EXIT WHEN curseur%NOTFOUND;
    IF ESTELEMENT(NomAttTmp, P_NUMENSATT_2)=0 THEN AJOUTERATT(NomAttTmp,Num);
    END IF;
  END LOOP;
  RETURN Num;
END SOUSTRACTIONATT;

--------------------------------------------------------------
-- II) 2) --
 create or replace FUNCTION CREERDF 
(
  P_CHAINEATT IN VARCHAR2 
) RETURN NUMBER IS 
Num NUMBER;
NumEnsGauche NUMBER;
NumEnsDroit NUMBER;
taille NUMBER;
str VARCHAR(255);
pos NUMBER;
BEGIN
  taille := LENGTH(P_CHAINEATT);
    pos := INSTR( P_CHAINEATT , '->');
    
    IF pos=0 THEN NumEnsGauche := CREERENSATTVIDE();
      
    ELSE
      str := SUBSTR(P_CHAINEATT, 0, pos-1);
      NumEnsGauche := CREERENSATT(str);
    END IF;

    str := SUBSTR(P_CHAINEATT, pos+2, taille-1);
    IF str = '' THEN NumEnsDroit := CREERENSATTVIDE();
    ELSE  NumEnsDroit := CREERENSATT(str);
    END IF;
  
  INSERT INTO DFs VALUES (NumDF.NextVal,NumEnsGauche, NumEnsDroit)
  RETURNING NumDF INTO Num;
  
  RETURN Num;
END CREERDF;

--------------------------------------------------------------

create or replace FUNCTION DF2CHAINE 
(
  P_NUMDF IN NUMBER 
) RETURN VARCHAR2 IS 
NumGauche DFS.NUMENSGAUCHE%type;
NumDroit DFS.NUMENSDROIT%type;
str VARCHAR(255);
BEGIN
  SELECT NumEnsGauche INTO NumGauche FROM DFS WHERE NUMDF=P_NUMDF;
  SELECT NumEnsDroit INTO NumDroit FROM DFS WHERE NUMDF=P_NUMDF;
  str := ENSATT2CHAINE(NumGauche) || '->' || ENSATT2CHAINE(NumDroit);
  RETURN str;
END DF2CHAINE;

--------------------------------------------------------------

create or replace FUNCTION ESTTRIVIALE 
(
  P_NUMDF IN NUMBER 
) RETURN NUMBER IS 
NumGauche DFS.NUMENSGAUCHE%type;
NumDroit DFS.NUMENSDROIT%type;
BEGIN
  SELECT NumEnsGauche INTO NumGauche FROM DFS WHERE NUMDF=P_NUMDF;
  SELECT NumEnsDroit INTO NumDroit FROM DFS WHERE NUMDF=P_NUMDF;
  IF ESTINCLUS(NumDroit,NumGauche)=1 THEN RETURN 1;
  END IF;
  RETURN 0;
END ESTTRIVIALE;

--------------------------------------------------------------

create or replace FUNCTION ESTPLUSFORTE 
(
  P_NUMDF1 IN NUMBER 
, P_NUMDF2 IN NUMBER 
) RETURN NUMBER IS 
NumGauche1 DFS.NUMENSGAUCHE%type;
NumDroit1 DFS.NUMENSDROIT%type;
NumGauche2 DFS.NUMENSGAUCHE%type;
NumDroit2 DFS.NUMENSDROIT%type;
BEGIN
  SELECT NumEnsGauche INTO NumGauche1 FROM DFS WHERE NUMDF=P_NUMDF1;
  SELECT NumEnsDroit INTO NumDroit1 FROM DFS WHERE NUMDF=P_NUMDF1;
  SELECT NumEnsGauche INTO NumGauche2 FROM DFS WHERE NUMDF=P_NUMDF2;
  SELECT NumEnsDroit INTO NumDroit2 FROM DFS WHERE NUMDF=P_NUMDF2;
  
  IF ESTINCLUS(NumDroit1,NumDroit2)=1 AND ESTINCLUS(NumGauche1,NumGauche2)=1 THEN RETURN 1;
  END IF;
  RETURN 0;
END ESTPLUSFORTE;

--------------------------------------------------------------
--- TESTS ---

SET SERVEROUTPUT ON
declare
str VARCHAR(255);
Begin
str :=DF2CHAINE(6);
DBMS_OUTPUT.PUT_LINE (str);
end;

SET SERVEROUTPUT ON
declare Num NUMBER(3);
str VARCHAR(255);
Begin Num := UNIONATT(70,81);
str :=ENSATT2CHAINE(Num);
DBMS_OUTPUT.PUT_LINE (str);
end;

SET SERVEROUTPUT ON
declare Num NUMBER(3);
str VARCHAR(255);
Begin Num := INTERSECTIONATT(84,1);
str :=ENSATT2CHAINE(Num);
DBMS_OUTPUT.PUT_LINE (str);
end;

SET SERVEROUTPUT ON
declare Num NUMBER(3);
str VARCHAR(255);
Begin Num := SOUSTRACTIONATT(1,85);
str :=ENSATT2CHAINE(Num);
DBMS_OUTPUT.PUT_LINE (str);
end;

SET SERVEROUTPUT ON
declare Num NUMBER(3);
str VARCHAR(255);
Begin Num := COPIEATT(85);
str :=ENSATT2CHAINE(Num);
DBMS_OUTPUT.PUT_LINE (str);
end;

SET SERVEROUTPUT ON
declare str VARCHAR(255);
Begin
str := ENSATT2CHAINE(81);
DBMS_OUTPUT.PUT_LINE (str);
end;


SET SERVEROUTPUT ON
declare resultat NUMBER;
Begin
resultat := ESTELEMENT('C', 81);
DBMS_OUTPUT.PUT_LINE (resultat);
resultat := ESTELEMENT('P', 81);
DBMS_OUTPUT.PUT_LINE (resultat);
end;

SET SERVEROUTPUT ON
declare resultat NUMBER;
Begin
resultat := ESTINCLUS(82, 81);
DBMS_OUTPUT.PUT_LINE (resultat);
resultat := ESTINCLUS(81, 82);
DBMS_OUTPUT.PUT_LINE (resultat);
end;

SET SERVEROUTPUT ON
declare resultat NUMBER;
Begin
resultat := ESTEGAL(82, 83);
DBMS_OUTPUT.PUT_LINE (resultat);
resultat := ESTEGAL(81, 82);
DBMS_OUTPUT.PUT_LINE (resultat);
end;

SET SERVEROUTPUT ON
declare resultat NUMBER;
Begin
resultat := ESTTRIVIALE(6);
DBMS_OUTPUT.PUT_LINE (resultat);
resultat := ESTTRIVIALE(7);
DBMS_OUTPUT.PUT_LINE (resultat);
end;

--------------------------------------------------------------

