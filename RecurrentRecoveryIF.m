%use to reconstruct recurrent connection matrix from network dynamics

clearvars;

warning('off','all')
A=load('A.txt');                     %recurrent connection matrix for validation

numIte =  load('numIt.txt');  %number of input stimuli

[height,width] = size(A);
o = ones(height,1);   


%extract information corresponding to desired row of mtx seek to recover

pfull = [];
mfull = [];
bfull = [];
vfull=[];

%load in ensemble of firing rate, mean voltage, input vectors

for i=1:numIte
    
    is2 = num2str(i);
    An = strcat('firingRates', is2, '.txt');
    m = load(An);
    
    An = strcat('forcing', is2, '.txt');
    p = load(An);
    
    An = strcat('volt', is2, '.txt');
    v = load(An);

    mfull = [mfull m];  
    
    pfull = [pfull p];

    vfull = [vfull v];
    
    bfull= [bfull ( v - p+m)];    
end

solnset = [];

for i=1:height
            
            row = bfull(i,:);  %recover one row at a time
     
            mfull2 =mfull';

            mfull2(:,i) = zeros(1,numIte);  %optional

            %use an l1 minimization solver; example uses the OMP algorihtm
            cand =(cs_omp(  row',mfull2,width));  
            
            solnset = [solnset; cand ];
    
end

%compute reconstruction error: use relative Frobenius norm
norm1 = norm(solnset - A, 'fro')/norm(A,'fro') 

save('Arec.txt', 'solnset','-ascii');

normL1 = norm(solnset(:) - A(:), 1)/(norm(A(:),1))
 
save('norm1.txt', 'norm1','-ascii');
save('normL1.txt', 'normL1','-ascii');

