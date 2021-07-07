%************************************************************************%
function hat_x=cs_omp(y,T_Mat,m)
% y=T_Mat*x, T_Mat is n-by-m
% y - measurements
% T_Mat - combination of random matrix and sparse representation basis
% m - size of the original signal
% the sparsity is length(y)/4, may adjust

n=length(y);

%controls sparsity and number of iterations      
s=floor(n/4);

%empty soln 
hat_x=zeros(1,m);   

%set of highly correlated columns
Aug_t=[];    

%residual: initialize with rhs
r_n=y;                                             

for times=1:s                                  

    %look at correlations bn residual and cols of samp mtx
    product=abs(T_Mat'*r_n);                 
    
    %find maximal correlation and its corresponding column
    [val,pos]=max(product);  
    
    %augment the set of highly correlated columns
    Aug_t=[Aug_t,T_Mat(:,pos)];  
    
    %don't consider this highly corr. column in future correlation computations
    T_Mat(:,pos)=zeros(n,1);      
    
    %solve min augt x-y
    aug_x=(Aug_t'*Aug_t)^(-1)*Aug_t'*y;   
    
    %compute new residual
    r_n=y-Aug_t*aug_x;                            
    pos_array(times)=pos;                          
    
end

%the value for the soln. estimate in component posarray_i is aug_x_i,
%rest entries 0; next residuals are orthog. to all highly corr columns
hat_x(pos_array)=aug_x;                           