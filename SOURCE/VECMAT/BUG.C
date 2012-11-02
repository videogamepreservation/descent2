To:           matt
From:         "Mike Kulas" <SERVER/MIKE>
Date:         20 Sep 93 10:49:26 
Subject:      Yet another piece of mail: vm_vec_normal testing
X-mailer:     Pegasus Mail v2.3 (R5).

This fragment of code:

{
    vms_vector  _v1,_v2,_v3,_v4;
    vm_vec_make(&_v1,1000,1000,1000);
    vm_vec_make(&_v2,1000,1000,2000);
    vm_vec_make(&_v3,1000,2000,2000);
    vm_vec_normal(&_v4,&_v1,&_v2,&_v3);

}

sets _v4 to:

    { 4806808, 4803124, 4803137 }

which seems wrong to me.

Am I doing something wrong?
ÿ