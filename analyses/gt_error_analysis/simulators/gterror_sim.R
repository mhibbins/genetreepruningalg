remove(list=ls())
suppressMessages(suppressWarnings(library(tidyverse, warn.conflicts = F, quietly = T)))
library(MASS, warn.conflicts = F, quietly = T)

sim_BM_gt_error <- function(t1, t2, N, n_traits) {
  
  
  ### Theory ###
  
  tau = t2 - t1
  scale = 2*N*0.0002 #assuming constant mu*simga2
  
  cov_AB <- (1 - exp(-tau/(2*N)))*(1 + 
		(tau/(2*N) - (1 - (tau/(2*N))/(exp(tau/(2*N))-1)))) + 
		(1/3)*exp(-tau/(2*N))

  cov_nonsister <- (1/3)*exp(-tau/(2*N))

  var = t1/(2*N) + (1 - exp(-tau/(2*N)))*(tau/(2*N) + 1) + 
	  exp(-tau/(2*N))*(tau/(2*N) + 1 + 1/3)

  var_covar <- scale*matrix(c(t2/(2*N), (t2-t1)/(2*N), 0,
			      (t2-t1)/(2*N), t2/(2*N), 0,
			      0, 0, t2/(2*N)),
			    nrow = 3, ncol = 3)

  
  ### Trait value simulations ### 
  
  traits <- mvrnorm(n = n_traits, mu = c(0, 0, 0), Sigma = var_covar)
  return(traits)
  
}

test_traits <- sim_BM_gt_error(4000, 50000, 16000, 100)
  
print(test_traits)
