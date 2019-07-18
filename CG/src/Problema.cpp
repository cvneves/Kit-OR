#include "Problema.h"

Problema::Problema(Data &d, double UB)
{
    this->data = d;
    this->bestInteger = UB;

    //Construçao do mestre

    env1 = IloEnv();
    masterModel = IloModel(env1);
    lambda = IloNumVarArray(env1, data.getNItems(), 0.0, IloInfinity);
    masterRanges = IloRangeArray(env1);

    //FO e nomes de lambda
    IloExpr sum(env1);
    for (int i = 0; i < data.getNItems(); i++)
    {
        char var_name[30];
        sprintf(var_name, "l%d", i);
        lambda[i].setName(var_name);
        sum += M * lambda[i];
        masterRanges.add(lambda[i] == 1);
    }

    masterModel.add(masterRanges);
    masterObj = IloMinimize(env1, sum);
    masterModel.add(masterObj);

    lambdaItens = std::vector<std::vector<bool>>(data.getNItems(), std::vector<bool>(data.getNItems(), false));

    for (int i = 0; i < data.getNItems(); i++)
    {
        lambdaItens[i][i] = true;
    }

    // master.setOut(env1.getNullStream());
}

std::pair<int, int> Problema::solve(Node &node)
{
    IloCplex master = IloCplex(masterModel);
    master.setOut(env1.getNullStream());

    //Construçao do subproblema
    IloEnv env2;
    IloModel pricingModel(env2);
    IloExpr somaMochila(env2);
    IloBoolVarArray x(env2, data.getNItems());

    for (int i = 0; i < data.getNItems(); i++)
    {
        somaMochila += data.getItemWeight(i) * x[i];
    }

    pricingModel.add(somaMochila <= data.getBinCapacity());
    IloObjective pricingObj = IloMinimize(env2);
    pricingModel.add(pricingObj);

    //Restriçoes dos itens juntos
    for (auto &p : node.juntos)
    {
        // // x_i = x_j
        pricingModel.add(x[p.first] == x[p.second]);

        // se lambda separa o par, eliminá-lo
        for (int i = data.getNItems(); i < lambdaItens.size(); i++)
        {
            //nenhum dos itens está no padrão, ignorar
            if (lambdaItens[i][p.first] == false && lambdaItens[i][p.second] == false)
            {
                continue;
            }
            //os dois itens estão no padrão, ignorar
            if (lambdaItens[i][p.first] == true && lambdaItens[i][p.second] == true)
            {
                continue;
            }
            //apenas um dos itens está no padrão
            lambda[i].setUB(0.0);
        }
    }

    //Restriçoes dos itens separados
    for (auto &p : node.separados)
    {
        // adicionar restriçao x_i + x_j <= 1
        pricingModel.add(x[p.first] + x[p.second] <= 1);

        // se lambda contém o par, eliminá-lo
        for (int i = data.getNItems(); i < lambdaItens.size(); i++)
        {
            if (lambdaItens[i][p.first] == true && lambdaItens[i][p.second] == true)
            {
                lambda[i].setUB(0.0);
            }
        }
    }

    // master.exportModel("modelo.lp");

    z = std::vector<std::vector<double>>(data.getNItems(), std::vector<double>(data.getNItems(), 0));

    //Resolver o mestre pela primeira vez
    try
    {
        master.solve();
    }
    catch (IloException &e)
    {
        std::cout << e << '\n';
    }

    while (1)
    {
        if (master.getCplexStatus() == IloCplex::Infeasible)
        {
            break;
        }

        IloCplex pricing(pricingModel);
        pricing.setOut(env2.getNullStream());

        IloNumArray pi(env1, data.getNItems());

        master.getDuals(pi, masterRanges);
        IloExpr somaPricing(env2);

        //FO do pricing a partir das variaveis duais

        for (int i = 0; i < data.getNItems(); i++)
        {
            // std::cout << pi[i] << " ";
            somaPricing -= pi[i] * x[i];
        }

        pricingObj.setExpr(somaPricing);

        try
        {
            pricing.solve();
        }
        catch (IloException &e)
        {
            std::cout << e << '\n';
        }

        pi.clear();
        pi.end();

        // podar no caso de pricing inviavel
        if (pricing.getStatus() == IloAlgorithm::Infeasible)
        {
            prune();
            // std::cout << "\nPricing inviavel\n";

            pricingModel.end();

            env2.end();

            master.clear();
            master.end();

            return {0, 0};
        }

        // std::cout << 1 + pricing.getObjValue() << "\n\n\n\n\n\n";

        // Verificar se o custo reduzido é negativo
        if (1 + pricing.getObjValue() <= -EPSILON)
        {
            // Adicionar nova coluna
            IloNumArray x_values(env2, data.getNItems());

            pricing.getValues(x_values, x);

            for (int i = 0; i < x_values.getSize(); i++)
            {
                if (x_values[i] > 0.9)
                {
                    x_values[i] = 1;
                }
                else
                {
                    x_values[i] = 0;
                }
            }

            IloNumVar lambdaVar(masterObj(1) + masterRanges(x_values), 0.0, IloInfinity);
            char var_name[30];
            sprintf(var_name, "l%d", lambda.getSize());
            lambdaVar.setName(var_name);

            lambda.add(lambdaVar);

            // Atualizar estrutura auxiliar
            std::vector<bool> itens(data.getNItems(), false);
            for (int i = 0; i < x_values.getSize(); i++)
            {
                if (x_values[i] > 1 - EPSILON)
                {
                    itens[i] = true;
                    std::cout << i << " ";
                }
            }
            std::cout << "\n";

            lambdaItens.push_back(itens);

            // master.exportModel("master2.lp");

            try
            {
                master.solve();
                // std::cout << "EEE??" << master.getCplexStatus() << "\n\n\n";
            }

            catch (IloException &e)
            {
                std::cout << e << "\n";
            }

            // std::cout << master.getObjValue() << "\n";

            x_values.clear();
            x_values.end();
            pricing.clear();
            pricing.end();
        }
        else
        {
            pricing.clear();
            pricing.end();
            break;
        }
    }

    // Verificar presença de variaveis artificiais na soluçao
    IloNumArray lambda_values(env1, lambda.getSize());
    master.getValues(lambda_values, lambda);

    if (!node.is_root)
    {
        for (int i = 0; i < data.getNItems(); i++)
        {
            if (lambda_values[i] > EPSILON)
            {
                prune();
                // std::cout << "\nVariavel artificial na soluçao\n";

                pricingModel.end();

                env2.end();
                lambda_values.end();

                master.clear();
                master.end();
                return {0, 0};
            }
        }
    }

    // Se o LB é pior que a melhor solução inteira, podar
    if (std::ceil(master.getObjValue() - EPSILON) - bestInteger >= 0)
    {
        if (!node.is_root)
        {

            prune();
            // std::cout << "\LB maior do que a melhor soluçao inteira\n";

            pricingModel.end();

            env2.end();
            lambda_values.end();

            master.clear();
            master.end();

            return {0, 0};
        }
    }

    // Regra de branching
    double deltaFrac = std::numeric_limits<double>::infinity();
    double tempDeltaFrac;

    std::pair<int, int> branchingPair;
    for (int i = 0; i < data.getNItems(); i++)
    {
        for (int j = i + 1; j < data.getNItems(); j++)
        {
            for (int k = data.getNItems(); k < lambdaItens.size(); k++)
            {
                //Soma lambda_k a z_ij se o par {i,j} está no padrão
                if (lambdaItens[k][i] == true && lambdaItens[k][j] == true)
                {
                    z[i][j] += lambda_values[k];
                    z[j][i] += lambda_values[k];
                }
            }

            tempDeltaFrac = std::abs(0.5 - z[i][j]);
            if (tempDeltaFrac < deltaFrac)
            {
                branchingPair = {i, j};
                deltaFrac = tempDeltaFrac;
            }
        }
    }

    // Verifica se a soluçao obtida é inteira

    // for (int i = 0; i < data.getNItems(); i++)
    // {
    //     std::cout << master.getValue(lambda[i]) << " ";
    // }

    if (std::abs(0.5 - deltaFrac) < EPSILON)
    {
        if (master.getObjValue() < bestInteger)
        {
            bestInteger = master.getObjValue();
        }

        prune();

        pricingModel.end();

        env2.end();
        // std::cout << "\nSoluçao inteira encontrada\n";

        master.clear();
        master.end();

        return {0, 0};
    }

    // master.exportModel("modelo.lp");

    lambda_values.end();
    // std::cout << master.getObjValue() << "\n";

    // Reverter UBs dos lambdas no fim do nó
    for (int i = 0; i < lambda.getSize(); i++)
    {
        lambda[i].setUB(IloInfinity);
    }

    // std::cout << "\npar: " << branchingPair.first << ", " << branchingPair.second << "\n";

    master.clear();
    master.end();

    return branchingPair;
}

void Problema::prune()
{
    for (int i = 0; i < lambda.getSize(); i++)
    {
        lambda[i].setUB(IloInfinity);
    }
    // std::cout << "PODAR\n";
}