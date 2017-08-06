X=0:0.01:1;
r=rand(1,101);
y=sin(2*pi*X)+(r-0.5);
theta= (rand(1,4)-0.5);
m=length(y);
alpha = 0.07;

for iter = 1:1:1000
   for j = 1:1:length(theta)
   sum = 0;
    for i = 1:1:m
        sum = sum + ( y(i) - (theta(1)*1 + theta(2)*X(i) + theta(3)*(X(i)^2) + theta(4)*(X(i)^3)) ) * (X(i)^(j-1)) ;  
    end
  
    theta(j) = theta(j) + alpha*sum;
    end
  
  error = 0;
  for i = 1:m
   
   hx(i) = theta(1) * 1 + theta(2) * X(i) + theta(3) * (X(i)^2) + theta(4) * (X(i)^3);
   error = error + (y(i) - hx(i))^2;
   
  end
  
  error
  err(iter) = error;
end
theta
plot(X,hx,'color','r'); hold on
plot(X,y,'color','b');
xlabel('x')
ylabel('h(X)')

%plot(err)
%axis([0 1000 0 50])
%xlabel('iterations')
%ylabel('error')

    
