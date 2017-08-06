clc
clear all
close all
action = zeros(10,1);
%rt = zeros(1000,1);
Q = [1:10];
Qstar = [1:10];
count = [1:10];
rmean = [1:1001];
rmean(1) = 0;

for i = 1:1:10
     Qstar(i) = normrnd(0,1);
end
epsvec = [1:3];
epsvec(1) = 0.1;
epsvec(2) = 0.01;
epsvec(3) = 0.001;

rmeanmat = zeros(3,1001);

for k=1:1:3
    eps = epsvec(k);
    for i=1:1:1000
        rmean(i+1) = 0;
    end
for m = 1:1:2000   
    for n = 1:1:10
        Q(n) = 0;
        count(n) = 0;
    end
    
    for j = 1:1:1000;
    
        prob = rand();
        if prob < (1-eps)
            [~,a] = max(Q);
        else 
            a = randi([1,10],1,1);
        end
        count(a) = count(a) + 1;
        r = normrnd(Qstar(a),1);
        rmean(j+1) = rmean(j+1) + r;
        Q(a) = Q(a)+ 1/count(a)*(r-Q(a));
      
    end
    

end
for i=1:1:1001
    rmeanmat(k,i) = rmean(i)/2000;
end
end

x=[0:1000];
figure
plot(x,rmeanmat(1,:),'c',x,rmeanmat(2,:),'b',x,rmeanmat(3,:),'r');