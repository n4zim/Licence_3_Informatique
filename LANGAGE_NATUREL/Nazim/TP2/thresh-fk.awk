BEGIN{
    prev="XXX";
}
{
    if(THRESHOLD == "") {
        print "Please define THRESHOLD when calling script!";
        exit;
    }
    if(prev==$1){
        count++;
    }
    else{
        count=0;
    } 
    if(count < THRESHOLD) {
        print $0;
    } 
    prev=$1;   
}
