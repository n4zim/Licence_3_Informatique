fact( entier $n )
{
si $n == 0 alors retour 1;
sinon retour $n * fact($n-1);
}

main()
{
afficher(fact(6));
}
