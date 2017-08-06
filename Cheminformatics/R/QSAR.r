# init global variables
X <- read.table("X.txt") #Einlesen der Matrix
a <- read.table("a.txt") #Einlesen des Ergebnis Vektors
options( warn = 1)
F <- lsfit(X, a) #Least squares fit
#Ausgabe der Ergebnisse
#In F$coefficients sind die Koeffiziente fuer das Modell gespeichert
print("Es ergeben sich die folgenden Parameter fuer die Ersetzung eines H durch ein anderes Atom (Reihenfolge jeweils H F Cl Br I Me):", quote=FALSE)
print(c("meta", 0.0, format(unname(F$coefficients[2:6]), digits=2)), quote=FALSE)
print(c("para", 0.0, format(unname(F$coefficients[7:11]), digits=2)), quote=FALSE)
print(c("mu =", format(unname(F$coefficients[1]), digits=2)), quote=FALSE)

#Berechnen der vorhergesagten Werte
matrix_dim <- dim(X)
y <- c()
for (i in 1:matrix_dim[1]) {
  res <- F$coefficients[1]
  for (j in 1:matrix_dim[2]) {
    res <- res + (X[i,j] * F$coefficients[1+j])
  }
  y[i] <- res
}
print(c("correlation R^2 =", format(cor(y, a)^2, digits=2)), quote=FALSE)
plot(a[,1], y, main="Linear model fit", ylab="predicted", xlab="observed");
print("y =")
print(y,digits=4)
paste(y,collapse=" ")
#function for cross validation
crossval <- function(no) {

  #get rid of line no
  if (no == 1) { 
    lY <- X[2:matrix_dim[1],]; 
    la <- a[2:matrix_dim[1],]; 
  } 
  else if(no == matrix_dim[1]) {
    lY <- X[1:matrix_dim[1]-1,]; 
    la <- a[1:matrix_dim[1]-1,];
  }
  else {
    lY <- rbind(X[1:(no-1),], X[(no+1):matrix_dim[1],]); 
    la <-     c(a[1:(no-1),], a[(no+1):matrix_dim[1],]);
  }

  #get fit
  lF <- lsfit(lY, la) #Least squares fit

  #get prediction
  res <- lF$coefficients[1];
  for (j in 1:matrix_dim[2]) 
    { res <- res + (X[no,j] * lF$coefficients[1+j]); }
  
  print(c("Prediction for", no, "is", unname(format(res,digits=5))), quote=FALSE);

  return(res);
}


#get test error using current test set
#idx is an array of indices of the current test set members
#with respect to the original input matrix
test_error <- function( idx ){
 
  X.test <- X[idx,]

  err <- 0
  scale <- 0
  pred <- c()
  mean_train <- mean( a[-idx,1] )
  #get fit
  lF <- lsfit(X[-idx,], a[-idx,1]) #Least squares fit

  #get prediction
  for (j in seq(length(idx))){ 
    pred <- c(pred,lF$coef[1] + lF$coeff[2:11] %*% t(X.test[j,]))
    err  <- err + (pred[j] - a[idx[j],1])^2 
    scale <- scale + ( a[idx[j],1] - mean_train )^2 
  }

  err <- 1 -( err / scale  )

  X11()
  plot( a[idx,1], pred , xaxp=c(7.5,9.5,5),yaxp=c(7.5,9.5,5), main="External validation",ylab="predicted", xlab="observed")
  return( err )
}

#validate a QSAR internally and externally
#using q^2. 
validate <- function() {
  
  #select indices of the test set
  #idx <- sample(seq(matrix_dim[1]),4)
  idx <- c(6,8,13,20)

  #Perform LOOCV on training set
  print( "***** LEAVE-ONE-OUT CROSS-VALIDATION *****")
  pred_loo <- c()
  for( i in seq(matrix_dim[1])[-idx] ){
    pred_loo <- c(pred_loo,crossval(i))}
  
  plot( a[-idx,1], pred_loo , xaxp=c(7.5,9.5,5),yaxp=c(7.5,9.5,5),main="Internal validation",ylab="predicted", xlab="observed")
  #calculate intenal q^2
  q2_int <- sum((pred_loo - a[-idx,1])^2)
  scale <- sum((a[-idx,1] - mean(a[-idx,1]))^2)
  q2_int <-  1 - (q2_int / scale)

  #calculate external q^2
  print( "***** EXTERNAL VALIDATION *****")
  print( paste("Calculating external q^2 using data point: ", idx ) )
  q2_ext <- test_error( idx )


  print( "***** VALIDATION SUMMARY *****")
  print(paste( "Internal q^2: ", q2_int ) )
  print(paste( "External q^2: ", q2_ext ) )
}
