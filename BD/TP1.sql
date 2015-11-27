CREATE TABLE EnsemblesAttributs (
  NumEnsAtt INTEGER,
  CONSTRAINT pk_EnsembleAttributs PRIMARY KEY (NumEnsAtt)
  ); 
  
CREATE TABLE EnsembleContientAttribut (
  NumEnsAtt INTEGER, 
  NomAtt VARCHAR(255),
  CONSTRAINT pk_EnsembleContientAttribut PRIMARY KEY (NumEnsAtt, NomAtt),
  CONSTRAINT fk_EnsContientAtt FOREIGN KEY (NumEnsAtt) REFERENCES EnsemblesAttributs(NumEnsAtt) ON DELETE CASCADE 
);

CREATE SEQUENCE NumEnsAtt; 

execute AJOUTERATT('A',3);