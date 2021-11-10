remove(list=ls())
suppressMessages(suppressWarnings(library(tidyverse, warn.conflicts = F, quietly = T)))
library(MASS, warn.conflicts = F, quietly = T)

sim_BM <- function(t1, t2, sigma2, n_traits) {
  
 
  cov_AB = sigma2*(t2-t1)
  var_all <- sigma2*t2
  
  var_covar <- matrix(c(var_all, cov_AB, 0,
                        cov_AB, var_all, 0,
                        0, 0, var_all), 
                        ncol = 3, byrow = TRUE)

    
    ### Trait value simulations ### 
  
  traits <- mvrnorm(n = n_traits, mu = c(0, 0, 0), Sigma = var_covar, empirical = TRUE)
  
  return(traits)
  
}

allpos = FALSE

test_traits <- sim_BM(0.6, 1.2, 1, 100)
  
print(test_traits)
