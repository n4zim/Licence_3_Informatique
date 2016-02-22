entier $c;

initialiser()
{
$c=0;
}

afficher( entier $n )
entier $i;
{
    $i = 0;
    tantque $i < $n faire {
        ecrire( $i );
        $i=$i+1;
    }    
}

incrementer( )
{
$c=$c+1;
}

main()
{
    initialiser();
    incrementer();
    afficher( $c );
}

  
