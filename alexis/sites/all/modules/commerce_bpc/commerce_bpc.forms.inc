<?php
/**
 * @file
 *   Form generation functions for the Commerce bulk product creation module
 */

/**
 * Build the Bulk Product Creation Form
 */
function commerce_bpc_create_bulk_form($form, &$form_state, $product_type) {
  // Display page 2 if $form_state['page_num'] == 2
  if (!empty($form_state['page_num']) && $form_state['page_num'] == 2) {
    return commerce_bpc_create_bulk_form_display_node($form, $form_state, $product_type);
  }
  // Otherwise we build page 1.
  $form_state['page_num'] = 1;

  $form['#parents'] = array();

  // Create a temporary product object attach fields
  $new_product = commerce_product_new($product_type);
  $language = !empty($new_product->language) ? $new_product->language : LANGUAGE_NONE;

  // Store the product type for later use
  $form['product_type'] = array(
    '#type' => 'value',
    '#value' => $product_type,
  );

  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('Bulk Product Creation Form'),
  );

  $form['product'] = array(
    '#type' => 'fieldset',
    '#title' => t('Product Info'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,

  );
  $form['combinations'] = array(
    '#type' => 'fieldset',
    '#title' => t('Combinations'),
    '#description' => t('A product will be created for every possible combination of the values you select here.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
    );

  $form['static_values'] = array(
    '#type' => 'fieldset',
    '#title' => t('Static values'),
    '#description' => t('The values of these fields will be shared by all generated products.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
    // needed for
    '#parents' => array('static_values'),
  );

  // We simply attach all fields to the 'static_values' fieldset, and then
  // allow modules to act on each of the fields.
  field_attach_form('commerce_product', $new_product, $form['static_values'], $form_state, $language);
  commerce_bpc_process_field_form_elements($form, $form_state);

  $form['product']['sku_fragment'] = array(
    '#type' => 'textfield',
    '#title' => t('SKU'),
    '#required' => TRUE,
    '#description' => t("The part of the SKU that is common to all products to be generated. The SKU of each individual product will be composed of this value together with values of the selected options."),
    '#size' => 60,
    '#maxlength' => 255,
    '#weight' => 0,
    '#process' => array('commerce_bpc_process_fragment_field'),
  );

  // $form['product']['sku_pattern'] = array(
  //   '#type' => 'textfield',
  //   '#title' => t('SKU Pattern'),
  //   '#required' => TRUE,
  //   '#description' => t("Enter a template used to generate the SKUs. You may use the tokens listed below."),
  //   '#size' => 100,
  //   '#weight' => 0,
  // );
  $form['product']['title_fragment'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#required' => TRUE,
    '#description' => t("The part of the product title that is common to all products to be generated. The SKU of each individual product will be composed of this value together with values of the selected options."),
    '#size' => 60,
    '#maxlength' => 255,
    '#weight' => 10,
    '#process' => array('commerce_bpc_process_fragment_field'),
  );
  // $form['product']['title_pattern'] = array(
  //   '#type' => 'textfield',
  //   '#title' => t('Title Pattern'),
  //   '#required' => TRUE,
  //   '#description' => t("Enter a template used to generate the Titles. You may use the tokens listed below."),
  //   '#size' => 100,
  //   '#weight' => 10,
  //   '#maxlength' => 255,
  // );


//  $form['product']['tokens'] = _commerce_bpc_show_tokens('bulk_product', $tokens);
//  $form['product']['tokens']['#weight'] = 30;

  $form['static_values']['status'] = array(
    '#type' => 'radios',
    '#title' => t('Status'),
    '#description' => t('Disabled products cannot be added to shopping carts and may be hidden in administrative product lists.'),
    '#options' => array(
      '1' => t('Active'),
      '0' => t('Disabled'),
    ),
    '#default_value' => 1,
    '#required' => TRUE,
    '#weight' => 200,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create products'),
    '#submit' => array('commerce_bpc_create_bulk_form_create_products_submit'),
    '#validate' => array('commerce_bpc_create_bulk_form_validate'),
  );

  if (variable_get('commerce_bpc_display_creation_method', 'wizard') == 'wizard') {
    // Show the Display Node option if applicable
    $display_nodes = commerce_bpc_get_node_types($product_type);
    if (!empty($display_nodes)) {
      $form['next'] = array(
        '#type' => 'submit',
        '#value' => 'Create products and Create display',
        '#submit' => array('commerce_bpc_create_bulk_form_create_products_submit'),
      );
    }
  }

  return $form;
}
/**
 * Implements hook_commerce_bpc_MODULE_NAME_form_element_alter().
 *
 * Moves the price field from the static values fieldset to the
 * product fieldset.
 */
function commerce_bpc_commerce_bpc_commerce_price_form_element_alter(&$form, &$form_state, &$path) {
  $field_widget = drupal_array_get_nested_value($form, $path);
  $lang = $field_widget['#language'];
  $field_name = $field_widget[$lang]['#field_name'];
  $form['product'][$field_name] = drupal_array_get_nested_value($form, $path);
  drupal_array_set_nested_value($form, $path, NULL);
  $path = array('product', 'commerce_price');
  $form['product']['commerce_price']['#weight'] = 20;


}
/**
 * Returns the form for the second page of form_example_tutorial_8().
 */
function commerce_bpc_create_bulk_form_display_node($form, &$form_state, $product_type) {
  $node_types = commerce_bpc_get_node_types($product_type);
  if (count($node_types) == 1) {
    // Redirect to the node/add form
    drupal_goto('node/add/' . str_replace('_', '-', reset($node_types)), array('query' => array('bulk_creation_id' => $form_state['bulk_creation_id'])));
  }
  else {
    $form['description'] = array(
      '#type' => 'item',
      '#title' => t('Bulk Product Creation Form - Choose Display Node'),
    );

    // Store the product type for later use
    $form['product_type'] = array(
      '#type' => 'value',
      '#value' => $product_type,
    );

    $form['node_type'] = array(
      '#type' => 'radios',
      '#title' => t('Display Node'),
      '#options' => $node_types,
      '#required' => TRUE,
      '#description' => t("Select which Display Node Type to use.
        Note that the Node Type must have a Product Reference Field with multiple values which can accept products of the chosen type to be considered as a candidate."),
    );

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Create node'),
      '#submit' => array('commerce_bpc_create_bulk_form_create_display_submit'),
      '#validate' => array(),
    );

    return $form;
  }
}

/**
 * Validation function for commerce_bpc_create_bulk_form().
 */
function commerce_bpc_create_bulk_form_validate($form, &$form_state) {
  // Check to make sure all of the SKUs to be generated are unique and not taken yet
  $skus = array();
  $data = array();
  $data['bulk_data'] = array(
    'product_type' => $form_state['values']['product_type'],
    'sku_fragment' => $form_state['values']['sku_fragment'],
    'title_fragment' => $form_state['values']['title_fragment'],
  );

  $combinations = commerce_bpc_get_combinations($form, $form_state);
  foreach ($combinations as $combination) {
    $data['bulk_data']['combination'] = $combination;
    $sku_pattern = variable_get('commerce_bpc_default_sku_pattern', '[bulk_defaults:entered_sku]--[bulk_defaults:combination_values]');

    $sku = token_replace($sku_pattern, $data);
    if (!commerce_product_validate_sku_unique($sku, '')) {
      form_set_error('sku_pattern', t('This pattern yielded one or more invalid SKUs, please use a different pattern. Check for token values that may have spaces.'));
      break;
    }
    elseif (in_array($sku, $skus)) {
      form_set_error('sku_pattern', t('This pattern yielded one or more duplicate SKUs, please use a different pattern.'));
      break;
    }
    else {
      $skus[] = $sku;
    }
  }
}

/**
 * The bulk product form submit handler
 */
function commerce_bpc_create_bulk_form_create_products_submit($form, &$form_state) {
  global $user;

  $product_type = $form_state['values']['product_type'];

  $combinations = commerce_bpc_get_combinations($form, $form_state);


  $data['bulk_data'] = array(
    'product_type' => $product_type,
    'sku_fragment' => $form_state['values']['sku_fragment'],
    'title_fragment' => $form_state['values']['title_fragment'],
  );

  $sku_pattern = variable_get('commerce_bpc_default_sku_pattern', '[bulk_defaults:entered_sku]--[bulk_defaults:combination_values]');
  $sku_pattern = token_replace($sku_pattern, $data);
  $title_pattern = variable_get('commerce_bpc_default_title_pattern', '[bulk_defaults:entered_title] ([bulk_defaults:combination_labels])');
  $title_pattern = token_replace($title_pattern, $data);
  $extras = array(
    'sku_pattern'   => $sku_pattern,
    'title_pattern' => $title_pattern,
    'uid' => $user->uid,
    'status' => $form_state['values']['static_values']['status'],
  );

  $static_values = $form_state['values']['static_values'];
  unset($static_values['status']);

  $static_values['commerce_price'] = $form_state['values']['commerce_price'];
  $product_ids = commerce_bpc_create_bulk_products($product_type, $combinations, $static_values, $extras);

  // Set a success message
  if (count($product_ids) > 0) {
    drupal_set_message(t("Successfully generated @count new products.", array('@count' => count($product_ids))));
  }
  switch (variable_get('commerce_bpc_display_creation_method')) {
    case 'auto':
      global $user;
      $node = new stdClass();
      $node->type = variable_get('commerce_bpc_display_auto_content_type', '');
      node_object_prepare($node);
      $node->language = LANGUAGE_NONE;
      $node->title = $form_state['values']['title_fragment'];
      // find the product reference field
      $instances = field_info_instances('node', $node->type);
      $ref_field_name = '';
      foreach ($instances as $name => $instance) {
        $field = field_info_field($name);
        if ($field['module'] == 'commerce_product_reference') {
          $ref_field_name = $name;
          break;
        }
      }
      foreach ($product_ids as $id) {
        $node->{$ref_field_name}[LANGUAGE_NONE][] = array('product_id' => $id);
      }
      drupal_alter('commerce_bpc_display_node', $node);
      node_save($node);
      switch (variable_get('commerce_bpc_display_auto_redirect', 'product listing')) {
        case 'display node' :
          $form_state['redirect'] = 'node/' . $node->nid;
          break;
        case 'custom' :
          $form_state['redirect'] = variable_get('commerce_bpc_display_auto_redirect_custom', '');
          break;
        case 'product listing' :
        default:
          $form_state['redirect'] = 'admin/commerce/products/list';
          break;
      }
      break;
    case 'wizard':
    default:
      // If selected, rebuild the form to let the user choose a display node
      if ($form_state['input']['op'] == 'Create products and Create display') {
        $form_state['product_type'] = $product_type;

        // Save the product IDs in the SESSION
        // Generate a id to make sure different bulk creations do not
        // interfere with each other
        $id = uniqid();
        $_SESSION['bulk_product_ids'][$id] = $product_ids;
        $form_state['bulk_creation_id'] = $id;

        // When form rebuilds, it will look at this to figure which
        // page to build.
        $form_state['page_num'] = 2;
        $form_state['rebuild'] = TRUE;
      }
      else {
        drupal_goto('admin/commerce/products');
      }

      break;
  }
}

/**
 * The bulk product choose display form submit handler
 */
function commerce_bpc_create_bulk_form_create_display_submit($form, &$form_state) {

  // Redirect to the node/add form
  drupal_goto('node/add/' . str_replace('_', '-', $form_state['values']['node_type']), array('query' => array('bulk_creation_id' => $form_state['bulk_creation_id'])));
}
function commerce_bpc_process_field_form_elements(&$form, &$form_state) {
  $product_type = $form['product_type']['#value'];
  foreach (element_children($form['static_values']) as $field_key) {
    $element = $form['static_values'][$field_key];
    $field_name = $element[$element['#language']]['#field_name'];
    $info = field_info_field($field_name);
    $instance_info = field_info_instance('commerce_product', $field_name, $product_type);
    // Filter out fields that are set to be hidden
    if (!_commerce_bpc_get_value($instance_info, array('commerce_bpc', 'show_field'), TRUE)) {
      unset($form['static_values'][$field_key]);
    }
    else {
      $types = array();
      $types[] = 'commerce_bpc_' . $info['type'] . '_form_element';
      if ($info['type'] != $info['module']) {
        $types[] = 'commerce_bpc_' . $info['module'] . '_form_element';
      }
      $types[] = 'commerce_bpc_form_element';
      $path = array('static_values', $field_key);
      drupal_alter($types, $form, $form_state, $path);
    }
  }
}
function commerce_bpc_process_fragment_field($element, &$form_state, &$form) {
  $product_type = $form['product_type']['#value'];
  $tokens = commerce_bpc_token_info_by_product_type($product_type);
  $samples = commerce_bpc_token_sample_values($product_type);

  $data['bulk_data']['combination'] = $samples['values'];
  $data['bulk_data']['product_type'] = $product_type;

  switch ($element['#name']) {
    case 'sku_fragment':
      $pattern = variable_get('commerce_bpc_default_sku_pattern', '[bulk_defaults:entered_sku]--[bulk_defaults:combination_values]');
      $token = '[bulk_defaults:entered_sku]';
      $target_id = 'sku_example';
      $label = t('Example variant SKU:');
      break;
    case 'title_fragment':
      $pattern = variable_get('commerce_bpc_default_title_pattern', '[bulk_defaults:entered_title] ([bulk_defaults:combination_labels])');
      $token = '[bulk_defaults:entered_title]';
      $target_id = 'title_example';
      $label = t('Example variant title:');
      break;
  }
  $pattern = token_replace($pattern, $data);
  $wrapper_id = $target_id . '_wrapper';
  $element['#field_suffix'] = '<small id="' . $wrapper_id . '">' . $label . ' <span id="' . $target_id . '"></span></small>';


  $js_settings = array(
    'type' => 'setting',
    'data' => array(
      'commerce_bpc' => array(
        '#' . $element['#id'] => array(
          'pattern' => $pattern,
          'target' => '#' . $target_id,
          'target_wrapper' => '#' . $wrapper_id,
          'token' => $token,
        ),
      ),
    ),
  );
  $element['#attached']['js'][] = drupal_get_path('module', 'commerce_bpc') . '/js/pattern.js';
  $element['#attached']['js'][] = $js_settings;

  return $element;
}